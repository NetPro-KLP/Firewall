#include "request.h"

#include <net/sock.h>

#include <linux/skbuff.h>

#include <linux/unistd.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/time.h>

#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/ip.h>
#include <linux/in.h>

#include <asm/processor.h>

#include "klp_protocol.h"
#include "ksocket.h"

//#define SERVER_ADDR		"127.0.0.1"
#define SERVER_ADDR	"172.16.100.61"
//#define SERVER_ADDR		"61.43.139.16"
#define SERVER_PORT	30000

void PrintData(klp_flow *data)
{
	printk("==============================================================\n");
	printk("source addr : %u|source port : %u|\ndestination addr : %u|destination port : %u|\ntcpudp : %u|\n", 
		data->key.saddr, data->key.src, data->key.daddr, data->key.dst, 
		data->key.tcpudp);
	printk("warn : %d|danger : %d|packet count : %d|total bytes : %d|\nstart time : %s|\nend time : %s\n", 
		data->warn, data->danger, data->packet_count, data->totalbytes,
		data->starttime,data->endtime);
	printk("==============================================================\n");
}

int SendHeader(ksocket_t sock_fd, char *code, void *data)
{
	struct sockaddr_in sock_addr;
	char buf[256]	= {0, };
	int len = 0;
	int data_count = 0;

	short int source;
	int saddr;
	int i;

	kgetsockname(sock_fd, (struct sockaddr *)&sock_addr, &len);
	source =sock_addr.sin_port;
    saddr = sock_addr.sin_addr.s_addr;

    if(strcmp(code, "ini") == 0 || strcmp(code, "alm") == 0)
    {
    	len = sprintf(buf, "%u|%u|%s|", saddr, 1, code);
    }
	else if(strcmp(code, "exp") == 0)
	{
		hash *data_table = (hash *)data;
		len = sprintf(buf, "%u|%u|%s|", saddr, data_table->count, code);
	}
	else
	{
		return -1;
	}
    
    for (i = len; i<48; i++)
		buf[i] = '-';
    data_count = ksend(sock_fd, buf, len+1, 0);

    //printk("%s %d\n", buf, data_count);
    
    return data_count;
}

int SendExpData(ksocket_t sock_fd, klp_flow *data)
{
	char buf[256] = {0, };
	int data_count = 0;
	int len, i;

	len = sprintf(buf, "%u|%u|%u|%u|%u|%u|%u|%u|%u|%s|%s|",
		data->key.saddr, data->key.src, data->key.daddr, data->key.dst, data->key.tcpudp,
		data->warn, data->danger, data->packet_count, data->totalbytes, data->starttime, data->endtime);

	for (i = len; i<119; i++)
		buf[i] = '-';

	len = strlen(buf);
	data_count = ksend(sock_fd, buf, len+1, 0);

	//printk("%s %d\n", buf, data_count);

	return data_count;
}

int SenderExp(hash *data_table, char *code)
{
	ksocket_t cli_fd;

	struct sockaddr_in srv_addr;
	char *temp = 0x00;

	int addr_len;
	int opt = 1;
	int i, r;

	//listNode *pCur = 0;
#ifdef KSOCKET_ADDR_SAFE
	mm_segment_t oldfs;
	old_fs = get_fs();
	set_fs(KERNEL_DS);
#endif

	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SERVER_PORT);
	srv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

	addr_len = sizeof(struct sockaddr_in);

	cli_fd = ksocket(AF_INET, SOCK_STREAM, 0);
	if(cli_fd == 0)
	{
		return -1;
	}

	if(ksetsockopt(cli_fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt)) < 0)
	{
		return -1;
	}

	if(kconnect(cli_fd, (struct sockaddr *)&srv_addr, addr_len) < 0)
	{
		return -1;
	}
	
	temp = inet_ntoa(&srv_addr.sin_addr);
	printk("connected to : %s %d\n", temp, ntohs(srv_addr.sin_port));
	kfree(temp);

	if(strcmp(code, "exp") == 0)
	{
		r = SendHeader(cli_fd, code, data_table);
		printk("hdr : %d\n", r);
		for(i=0; i<HASH_SIZE; i++)
	    {
	    	listNode *cur = data_table->item[i].head;
	        while(cur)
	        {
	        	r = SendExpData(cli_fd, &(cur->data));
	        	if (r <= 0)	continue;

	        	printk("payload : %d\n", r);
	        	PrintData(&(cur->data));
	        	cur = cur->next;
	        }
	    }
	}

	kclose(cli_fd);

#ifdef KSOCKET_ADDR_SAFE
	set_fs(old_fs);
#endif
	
	return 0;
}