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

//#define SERVER_ADDR	"172.16.100.61"
#define SERVER_ADDR		"61.43.139.16"
#define SERVER_PORT	30000

#define TIMESTEP	5

extern hash table;

hash expired_table;

extern rwlock_t exp_lock;
int base_time = 0;

unsigned int GetCurrentTime(void)
{
	struct timespec ts;
	getnstimeofday(&ts);
	return ts.tv_sec;
}
int SendExpHeader(klp_socket_t sock_fd, hash *data_table);
int SendData(klp_socket_t sock_fd, klp_flow *data);
int Sender(hash *table);

int TimeExpired(void)
{
	///////////////////////////////////////////////////////
	if( (GetCurrentTime() - base_time) < TIMESTEP )
	{
		return 1;
	}
	else
	{
		base_time = GetCurrentTime();
	}
	printk("exp\n");
	// copy move hash table
	
	if(table.count > 0)
	{
		
		printk("hash move\n");
		expired_table.count = table.count;
		expired_table.item = table.item;

		InitHash(&table);				
		
		// send hash to system backend
		Sender(&expired_table);
		//PrintkHash(&expired_table);

		//write_lock(&exp_lock);
		printk("destroy \n");
		DestroyHash(&expired_table);
		//write_unlock(&exp_lock);
	}
	else
	{
		return 2;
	}
	

	return 0;
}

void PrintData(klp_flow *data)
{
	printk("%ud:%ud:%ud:%ud:%ud:%ud:", 
		data->key.saddr, data->key.src, data->key.daddr, data->key.dst, 
		data->key.protocol, data->key.tcpudp);
	printk("%d:%d:%d:%d:%s:%s\n", 
		data->warn, data->danger, data->packet_count, data->totalbytes,
		data->starttime,data->endtime);
}
int SendExpHeader(klp_socket_t sock_fd, hash *data_table)
{
	struct sockaddr_in sock_addr;

	char code[] = "exp";
	int addr_len = 0;
	int data_count = 0;

	short int source;
	int saddr;

	klp_getsockname(sock_fd, (struct sockaddr *)&sock_addr, &addr_len);
	source =sock_addr.sin_port;
    saddr = sock_addr.sin_addr.s_addr;

	data_count += klp_write(sock_fd, (char*)&saddr, sizeof(int), 0);
    data_count += klp_write(sock_fd, (char*)&(data_table->count), sizeof(int), 0);
    data_count += klp_write(sock_fd, code, sizeof(int), 0);
    
    if(data_count < 3*sizeof(int))
    	return -1;
    else
    	return 0;
}

int SendData(klp_socket_t sock_fd, klp_flow *data)
{
	int data_count = 0;
	data_count += klp_write(sock_fd, (char*)&data->key.saddr, sizeof(int), 0);
	data_count += klp_write(sock_fd, (char*)&data->key.src, sizeof(short int), 0);
	data_count += klp_write(sock_fd, (char*)&data->key.daddr, sizeof(int), 0);
	data_count += klp_write(sock_fd, (char*)&data->key.dst, sizeof(short int), 0);
	data_count += klp_write(sock_fd, (char*)&data->key.protocol, sizeof(short int), 0);
	data_count += klp_write(sock_fd, (char*)&data->key.tcpudp, sizeof(char), 0);
	data_count += klp_write(sock_fd, (char*)&data->warn, sizeof(int), 0);
	data_count += klp_write(sock_fd, (char*)&data->danger, sizeof(int), 0);
	data_count += klp_write(sock_fd, (char*)&data->packet_count, sizeof(int), 0);
	data_count += klp_write(sock_fd, (char*)&data->totalbytes, sizeof(int), 0);
	data_count += klp_write(sock_fd, data->starttime, sizeof(char)*20, 0);
	data_count += klp_write(sock_fd, data->endtime, sizeof(char)*20, 0);

	if(data_count < 71)
		return -1;
	else
		return 0;
}

int Sender(hash *data_table)
{
	klp_socket_t cli_fd;

	struct sockaddr_in srv_addr;
	char *temp = 0x00;

	int addr_len;
	int i;

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
	if(cli_fd == 0)
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

    
//	read_lock(&exp_lock);
	
//	write_lock(&exp_lock);
    for(i=0; i<HASH_SIZE; i++)
    {
    	listNode *cur = data_table->item[i].head;
        while(cur)
        {
        	SendExpHeader(cli_fd, data_table);
        	SendData(cli_fd, &(cur->data));
        	PrintData(&(cur->data));
        	cur = cur->next;
        }
    }
    //write_unlock(&exp_lock);
//	read_unlock(&exp_lock);

	klp_close(cli_fd);

#ifdef KLP_SOCKET_ADDR_SAFE
	set_fs(old_fs);
#endif
	
	return 0;
}

int start_expired(void *arg)
{
	while(1)
	{
		if(kthread_should_stop())
			break;
		else
			schedule();

		TimeExpired();
	}
	return 0;
}

void exit_expired(void)
{

}
/*
MODULE_LICENSE("GPL");
MODULE_AUTHOR("jangsoopark");
MODULE_DESCRIPTION("soma firewall kernel module");*/