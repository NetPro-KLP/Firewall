#include "listener.h"

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

#define SERVER_PORT		30001

int listener(void)
{
	klp_socket_t sockfd_srv, sockfd_cli;

	struct sockaddr_in addr_srv;
	struct sockaddr_in addr_cli;

	char buf[512], *tmp;
	int addr_len, len;

#ifdef KSOCKET_ADDR_SAFE
		mm_segment_t old_fs;
		old_fs = get_fs();
		set_fs(KERNEL_DS);
#endif
	
	sockfd_srv = sockfd_cli = 0;
	memset(&addr_cli, 0, sizeof(addr_cli));
	memset(&addr_srv, 0, sizeof(addr_srv));

	addr_srv.sin_family = AF_INET;
	addr_srv.sin_port = htons(SERVER_PORT);
	addr_srv.sin_addr.s_addr = INADDR_ANY;
	addr_len = sizeof(struct sockaddr_in);
	
	sockfd_srv = klp_socket(AF_INET, SOCK_STREAM, 0);
	printk("sockfd_srv = 0x%p\n", sockfd_srv);
	if (sockfd_srv == NULL)
	{
		printk("socket failed\n");
		return -1;
	}
	if (klp_bind(sockfd_srv, (struct sockaddr *)&addr_srv, addr_len) < 0)
	{
		printk("bind failed\n");
		return -1;
	}
	if (klp_listen(sockfd_srv, 10) < 0)
	{
		printk("listen failed\n");
		return -1;
	}

	while(1)
	{
		sockfd_cli = klp_accept(sockfd_srv, (struct sockaddr *)&addr_cli, &addr_len);
		if (sockfd_cli == NULL)
		{
			printk("accept failed\n");
			return -1;
		}
		else
		{
			printk("sockfd_cli = 0x%p\n", sockfd_cli);
		}
		
		tmp = klp_inet_ntoa(&addr_cli.sin_addr);
		printk("got connected from : %s %d\n", tmp, ntohs(addr_cli.sin_port));
		kfree(tmp);

		memset(buf, 0, sizeof(buf));
		klp_read(sockfd_cli, buf, 512, 0);

		
		klp_close(sockfd_cli);
	}
	
	klp_close(sockfd_srv);
#ifdef KSOCKET_ADDR_SAFE
		set_fs(old_fs);
#endif
	
	return 0;
}