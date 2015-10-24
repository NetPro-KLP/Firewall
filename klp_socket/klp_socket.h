#ifndef __KLP_SOCKET_H__
#define __KLP_SOCKET_H__

#include <linux/types.h>

struct socket;
struct sockaddr;
struct in_addr;

typedef struct socket *klp_socket_t;

klp_socket_t klp_socket(int domain, int type, int protocol);

int klp_bind(klp_socket_t socket, struct sockaddr *address, int address_len);
int klp_listen(klp_socket_t socket, int backlog);
int klp_connect(klp_socket_t socket, struct sockaddr *address, int  address_len);
klp_socket_t klp_accept(klp_socket_t socket, struct sockaddr *address, int *address_len);

ssize_t klp_read(klp_socket_t socket, void *buffer, ssize_t length, int flags);
ssize_t klp_write(klp_socket_t socket, const void *buffer, size_t length, int flags);

int klp_shutdown(klp_socket_t socket, int how);
int klp_close(klp_socket_t socket);

int klp_getsockname(klp_socket_t socket, struct sockaddr *address, int address_len);
int klp_getpeername(klp_socket_t socket, struct sockaddr *address, int address_len);

unsigned int inet_addr(char *ip);
char *inet_ntoa(struct in_addr *in);

#endif