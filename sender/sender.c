#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/types.h>
#include <linux/fcntl.h>

#include <linux/string.h>
#include <linux/socket.h>
#include <linux/net.h>

#include <net/sock.h>
#include <asm/processor.h>
#include <asm/uaccess.h>

#include <linux/in.h>

#include <linux/kthread.h>
#include <linux/ioport.h>

#include "klp_socket.h"
#include "klp_protocol.h"

#define BUFSIZE	512

//#define SERVER_ADDR "127.0.0.1"
//#define SERVER_PORT	30000
#define SERVER_ADDR "216.58.221.100"
#define SERVER_PORT	80

struct task_struct *ts;

int sender(void *data)
{
	klp_socket_t cli_fd;
	struct sockaddr_in srv_addr;

	char buf[BUFSIZE] = {0, };
	char *temp;
	int addr_len;

#ifdef KLP_SOCKET_ADDR_SAFE
	mm_segment_t old_fs;
	old_fs = get_fs();
	set_fs(KERNEL_DS);
#endif

	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SERVER_PORT);
	srv_addr.sin_addr.s_addr = klp_inet_addr(SERVER_ADDR);

	addr_len = sizeof(struct sockaddr_in);

	cli_fd = klp_socket(AF_INET, SOCK_STREAM, 0);
	if(cli_fd == 0x00)
	{
		return -1;
	}
	if(klp_connect(cli_fd, (struct sockaddr*)&srv_addr, addr_len) < 0)
	{
		return -1;
	}

	temp = klp_inet_ntoa(&srv_addr.sin_addr);
	printk("connected to : %s %d\n", temp, ntohs(srv_addr.sin_port));
	kfree(temp);

//	klp_write(cli_fd, req, strlen(req), 0);
//	klp_read(cli_fd, buf, BUFSIZE, 0);
//	printk("message : %s\n", buf);
 
 	printk("haha\n");
	klp_close(cli_fd);

#ifdef KLP_SOCKET_ADDR_SAFE
	set_fs(old_fs);
#endif

	return 0;
}

static int sender_init(void)
{
	ts = kthread_run(sender, 0x00, "firewall_sender");
	return 0;
}

static void sender_exit(void)
{
	if(ts)
	{
		kthread_stop(current);
	}	

}

module_init(sender_init);
module_exit(sender_exit);


MODULE_LICENSE("GPL");