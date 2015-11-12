#include "klp_socket.h"

#include <linux/module.h>
#include <linux/string.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <linux/uio.h>
#include <linux/in.h>

#include <net/sock.h>

#include <asm/processor.h>
#include <asm/uaccess.h>

#define NAME 		"klp_socket"
#define AUTHOR		"jangsoo park (software maestro 6th)"
#define DESCRIPT	"kernel socket library for version 3.19.x"

klp_socket_t klp_socket(int domain, int type, int protocol)
{
	struct socket *sk = 0x00;
	int ret = 0;

	ret = sock_create(domain, type, protocol, &sk);
	if(ret < 0)
	{
		return 0x00;
	}

	return sk;
}

int klp_bind(klp_socket_t socket, struct sockaddr *address, int address_len)
{
	struct socket *sk = 0x00;
	int ret = 0;

	sk = (struct socket *)socket;
	ret = sk->ops->bind(sk, address, address_len);

	return ret;
}

int klp_listen(klp_socket_t socket, int backlog)
{
	struct socket *sk;
	int ret;

	sk = (struct socket *)socket;
	if((unsigned)backlog > SOMAXCONN)
		backlog = SOMAXCONN;

	ret = sk->ops->listen(sk, backlog);

	return ret;
}

int klp_connect(klp_socket_t socket, struct sockaddr *address, int  address_len)
{
	struct socket *sk;
	int ret;

	sk = (struct socket *)socket;
	ret = sk->ops->connect(sk, address, address_len, 0/*sk->file->f_file->f_flags*/);

	return ret;
}

klp_socket_t klp_accept(klp_socket_t socket, struct sockaddr *address, int *address_len)
{
	struct socket *sk = 0x00;
	struct socket *new_sk = 0x00;
	int ret;

	sk = (struct socket *)socket;

	ret = sock_create(sk->sk->sk_family, sk->type, sk->sk->sk_protocol, &new_sk);
	if(ret < 0)
		return 0x00;
	if(!new_sk)
		return 0x00;

	new_sk->type = sk->type;
	new_sk->ops = sk->ops;

	ret = sk->ops->accept(sk, new_sk, 0/*sk->file->f_flags*/);
	if(ret < 0)
	{
		sock_release(new_sk);
		return 0x00;
	}
	if(address)
	{
		ret = new_sk->ops->getname(new_sk, address, address_len, 2);
		if(ret < 0)
		{
			sock_release(new_sk);
			return 0x00;
		}
	}
	return new_sk;
}

int klp_shutdown(klp_socket_t socket, int how)
{
	struct socket *sk = 0x00;
	int ret = 0;

	sk = (struct socket *)socket;
	if (sk)
		ret = sk->ops->shutdown(sk, how);

	return ret;
}

int klp_close(klp_socket_t socket)
{
	struct socket *sk;
	int ret;

	sk = (struct socket *)socket;
	ret = sk->ops->release(sk);

	if(sk)
		sock_release(sk);

	return ret;
}

ssize_t klp_read(klp_socket_t socket, void *buffer, ssize_t length, int flags)
{
	struct socket *sk;
	struct msghdr msg;
	struct iov_iter iovi;
	struct iovec iov;
	
	int ret;

#ifndef KLP_SOCKET_ADDR_SAFE
	mm_segment_t old_fs;
#endif

	sk = (struct socket *)socket;

	iov.iov_base = (void *) buffer;
	iov.iov_len = (__kernel_size_t)length;

	msg.msg_name = 0x00;
	msg.msg_namelen = 0;
	
	iovi.iov = &iov;
	msg.msg_iter = iovi;
	msg.msg_control = 0x00;
	msg.msg_controllen = 0;

#ifndef KLP_SOCKET_ADDR_SAFE
	old_fs = get_fs();
	set_fs(KERNEL_DS);
#endif

	ret = sock_recvmsg(sk, &msg, length, flags);

#ifndef KLP_SOCKET_ADDR_SAFE
	set_fs(old_fs);
#endif

	return ret;
}

ssize_t klp_write(klp_socket_t socket, const void *buffer, size_t length, int flags)
{
	struct socket *sk = 0x00;
	struct msghdr msg;
	struct iov_iter iovi;
	struct iovec iov;
	int len;

#ifndef KLP_SOCKET_ADDR_SAFE
	mm_segment_t old_fs;
#endif

	sk = (struct socket *)socket;

	iov.iov_base = (void *)buffer;
	iov.iov_len = (__kernel_size_t)length;

	msg.msg_name = 0x00;
	msg.msg_namelen = 0;

	iovi.iov = &iov;
	msg.msg_iter = iovi;

	msg.msg_control = 0x00;
	msg.msg_controllen = 0;

	msg.msg_flags = flags;

#ifndef KLP_SOCKET_ADDR_SAFE
	old_fs = get_fs();
	set_fs(KERNEL_DS);
#endif

	len = sock_sendmsg(sk, &msg, length);

#ifndef KLP_SOCKET_ADDR_SAFE
	set_fs(old_fs);
#endif

	return len;
}

unsigned int klp_inet_addr(char *ip)
{
	int a, b, c, d;
	char addr[4];

	sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d);
	addr[0] = a;
	addr[1] = b;
	addr[2] = c;
	addr[3] = d;

	return *(unsigned int *)addr;
}

char *klp_inet_ntoa(struct in_addr *in)
{
	char *str_ip = 0x00;
	unsigned int int_ip = 0;

	str_ip = kmalloc(16*sizeof(char), GFP_KERNEL);
	if(!str_ip)
		return NULL;
	else
		memset(str_ip, 0, 16);

	int_ip = in->s_addr;
	sprintf(str_ip, "%d.%d.%d.%d", 	(int_ip     ) & 0xff,
									(int_ip >> 8 ) & 0xff,
									(int_ip >> 16) & 0xff,
									(int_ip >> 24) & 0xff);
	return str_ip;
}

int klp_setsockopt(klp_socket_t socket, int level, int optname, void *optval, int optlen)
{
	struct socket *sk;
	int ret;
#ifndef KLP_SOCKET_ADDR_SAFE
	mm_segment_t old_fs;
#endif

	sk = (struct socket *)socket;

#ifndef KLP_SOCKET_ADDR_SAFE
	old_fs = get_fs();
	set_fs(KERNEL_DS);
#endif

	if (level == SOL_SOCKET)
		ret = sock_setsockopt(sk, level, optname, optval, optlen);
	else
		ret = sk->ops->setsockopt(sk, level, optname, optval, optlen);

#ifndef KLP_SOCKET_ADDR_SAFE	
	set_fs(old_fs);
#endif

	return ret;
}

int klp_getsockname(klp_socket_t socket, struct sockaddr *address, int *address_len)
{
	struct socket *sk;
	int ret;
	
	sk = (struct socket *)socket;
	ret = sk->ops->getname(sk, address, address_len, 0);
	
	return ret;
}

int klp_getpeername(klp_socket_t socket, struct sockaddr *address, int *address_len)
{
	struct socket *sk;
	int ret;
	
	sk = (struct socket *)socket;
	ret = sk->ops->getname(sk, address, address_len, 1);
	
	return ret;
}
/*
static int klp_socket_init(void)
{
	printk("klp_socket init\n");
	return 0;
}

static void klp_socket_exit(void)
{
	printk("klp_socket exit\n");
}

module_init(klp_socket_init);
module_exit(klp_socket_exit);

EXPORT_SYMBOL(klp_socket);
EXPORT_SYMBOL(klp_bind);
EXPORT_SYMBOL(klp_listen);
EXPORT_SYMBOL(klp_connect);
EXPORT_SYMBOL(klp_accept);
EXPORT_SYMBOL(klp_read);
EXPORT_SYMBOL(klp_write);
EXPORT_SYMBOL(klp_inet_addr);
EXPORT_SYMBOL(klp_inet_ntoa);

MODULE_AUTHOR(AUTHOR);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(DESCRIPT);
*/