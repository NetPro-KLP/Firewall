#include <linux/init.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/types.h>
#include <linux/fcntl.h>

#include <linux/module.h>

#include <asm/processor.h>
#include <linux/kthread.h>
#include <linux/sched.h>

#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/ip.h>

#include <linux/string.h>
#include <linux/timer.h>
#include <linux/time.h>

#include "klp_protocol.h"
#include "trie.h"
#include "hash.h"


// net flow table
hash table;	

// net filter structure 
static struct nf_hook_ops netfilter_ops;
struct sk_buff *sock_buff;

unsigned long inet_aton(const char *str)
{
	unsigned long result = 0;
	unsigned int iaddr[4] = {0, };
	unsigned char addr[4] = {0, };

	int i;
	sscanf(str, "%d.%d.%d.%d ", iaddr, iaddr + 1, iaddr + 2, iaddr + 3);

	for(i = 0; i < 4; i++)
	{
		addr[i] = (char)iaddr[i];
	}

	for(i = 3; i > 0; i--)
	{
		result |= addr[i];
		result <<= 8;
	}

	result |= addr[0];
	return result;
}

char *current_time(char *strtime)
{
	struct timeval tv;
	struct tm tm;

	do_gettimeofday(&tv);
	time_to_tm(tv.tv_sec, 0, &tm);

	sprintf(strtime, "%04ld-%02d-%02d %02d:%02d:%02d", 
		tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	return strtime;
}

unsigned long inet_aton(const char* );
char *current_time(char *strtime);

// main packet hooking funciton
unsigned int main_hook(unsigned int hooknum, struct sk_buff *skb,
						const struct net_device *in, const struct net_device *out,
						int (*okfn)(struct sk_buff *))
{
	char *data = 0; // packet payload.

	struct iphdr *iph = ip_hdr(skb);
	
	// source ip address, destination ip address
	unsigned long saddr = 0, daddr = 0;

	// source port number, destination port number
	unsigned short source = 0, dest = 0;

	// get tcp header, get udp header
	struct tcphdr *tcph = (struct tcphdr*)((char*)iph + sizeof(struct iphdr));
	struct udphdr *udph = (struct udphdr*)((char*)iph + sizeof(struct iphdr));

	klp_key key ;
	klp_flow flow ;
	char cur_time[20] = {0, };

	listNode *search = 0x00;

	memset((char*)&key, 0, sizeof(key));
	memset((char*)&flow, 0, sizeof(flow));
	// source and destination address
	saddr = iph->saddr;
	daddr = iph->daddr;

	// source and destination port from tcp header
	source = htons(tcph->source);
	dest = htons(tcph->dest);

	key.saddr = saddr;
	key.daddr = daddr;
	key.src = source;
	key.dst = dest;
	key.protocol = source;
	key.tcpudp = iph->protocol == IPPROTO_TCP ? 1 : 0;

	flow.key = key;
	
	current_time(cur_time);
	strcpy(flow.starttime, cur_time);

	printk("<1>default source : %u \t dest : %u\n %s\n", source, dest, data);

	// when communication protocol is udp, get payload
	if(iph->protocol == IPPROTO_UDP)
	{
		data = (char*)udph + sizeof(*udph);
		printk("<1> udp source : %u \t dest : %u\n %s\n", source, dest, data);
	}// when communication protocol is tcp, get payload
	else if(iph->protocol == IPPROTO_TCP)
	{
		data = (char*)tcph + sizeof(*tcph);

		printk("<1>tcp source : %u \t dest : %u\n %s\n", source, dest, data);
	}

	search = SearchHash(&table, &flow);
	if(search)
	{
		search->data.packet_count++;
		strcpy(search->data.endtime, cur_time);
	}
	else
	{
		InsertHash(&table, &flow);
	}
	/////////////////////////////////////////////////////////////////////////

	return NF_ACCEPT;
}

static int init_hook(void)
{
	netfilter_ops.hook = main_hook;
	netfilter_ops.pf = PF_INET;
	netfilter_ops.hooknum = NF_INET_PRE_ROUTING;
	netfilter_ops.priority = 1;
	
	InitHash(&table);
	nf_register_hook(&netfilter_ops);
	
	while(1)
	{
		if(kthread_should_stop())
			break;
		else
			schedule();
	}

	return 0;
}

void exit_hook(void)
{
	PrintkHash(&table);
	DestroyHash(&table);

	nf_unregister_hook(&netfilter_ops);
}

/*int init_module(void)
{
	kthread_run(init_hook);
	return 0;
}
void cleanup_modules(void)
{
	kthread_stop(init_hook);

}
module_init(init_modules);
module_exit(cleanup_modules);
*/
EXPORT_SYMBOL(table);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("jangsoopark");
MODULE_DESCRIPTION("soma firewall kernel module");