#include "expired.h"

//#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/unistd.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/fs.h>

#include <linux/kthread.h>
#include <linux/sched.h>

#include <linux/netfilter_ipv4.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>



#include <linux/string.h>
#include <linux/timer.h>
#include <linux/time.h>

#include <asm/processor.h>

#include "klp_protocol.h"
#include "klp_socket.h"

#include "request.h"

#include "hash.h"

#define TIMESTEP	5

extern hash table;

hash expired_table;

int base_time = 0;

unsigned int GetCurrentTime(void)
{
	struct timespec ts;
	getnstimeofday(&ts);
	return ts.tv_sec;
}

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
		SenderExp(&expired_table, "exp");
		//PrintkHash(&expired_table);

		printk("destroy \n");
		DestroyHash(&expired_table);
		base_time = GetCurrentTime();	
	}
	else
	{
		return 2;
	}
	

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