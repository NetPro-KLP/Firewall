#include "expired.h"

//#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/unistd.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/fs.h>

#include <linux/spinlock_types.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/sched.h>

#include <linux/netfilter_ipv4.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>

#include <linux/skbuff.h>
#include <linux/socket.h>

#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/ip.h>
#include <linux/in.h>

#include <net/sock.h>

#include <linux/string.h>
#include <linux/timer.h>
#include <linux/time.h>

#include <asm/processor.h>

#include "klp_protocol.h"
#include "klp_socket.h"

#include "hash.h"

#define SERVER_ADDR	"172.16.100.61"
#define SERVER_PORT	30000

#define TIMESTEP	5

extern hash table;

hash expired_table;

rwlock_t exp_lock;
int base_time = 0;

unsigned int GetCurrentTime(void)
{
	struct timespec ts;
	getnstimeofday(&ts);
	return ts.tv_sec;
}

int SendFlowData(hash *table);

int TimeExpired(void)
{
	while(1)
	{
		if(kthread_should_stop())
			break;
		else
			schedule();

	///////////////////////////////////////////////////////
		if( (GetCurrentTime() - base_time) < TIMESTEP )
		{
			continue;
		}
		else
		{
			// copy move hash table
			read_lock(&exp_lock);
			if(table.count > 0)
			{
				expired_table.count = table.count;
				expired_table.item = table.item;
				InitHash(&table);				
			}
			read_unlock(&exp_lock);

			// send hash to system backend
			write_lock(&exp_lock);
			SendFlowData(&expired_table);
			DestroyHash(&table);
			write_unlock(&exp_lock);

			base_time = GetCurrentTime();
		}
	}
	return 0;
}

int SendFlowData(hash *table)
{
	klp_socket_t cli_fd;

	struct sockaddr_in srv_addr;
	char *temp = 0x00;
	int addr_len;

	//listNode *pCur = 0;
#ifdef KLP_SOCKET_ADDR_SAFE
	mm_segment_t oldfs;
	old_fs = get_fs();
	set_fs(KERNEL_DS);
#endif

	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SERVER_PORT);
	srv_addr.sin_addr.s_addr = klp_inet_addr(SERVER_ADDR);

	addr_len = sizeof(struct sockaddr_in);

	cli_fd = klp_socket(AF_INET, SOCK_STREAM, 0);
	if(cli_fd)
	{
		return -1;
	}

	if(klp_connect(cli_fd, (struct sockaddr *)&srv_addr, addr_len) < 0)
	{
		return -1;
	}
	temp = klp_inet_ntoa(&srv_addr.sin_addr);
	printk("connected to : %s %d\n", temp, ntohs(srv_addr.sin_port));
	kfree(temp);


	PrintkHash(table);


	klp_close(cli_fd);

#ifdef KLP_SOCKET_ADDR_SAFE
	set_fs(old_fs);
#endif
	
	return 0;
}

int start_expired(void *arg)
{
	rwlock_init(&exp_lock);
	TimeExpired();

	return 0;
}

void exit_expired(void)
{

}
/*
MODULE_LICENSE("GPL");
MODULE_AUTHOR("jangsoopark");
MODULE_DESCRIPTION("soma firewall kernel module");*/