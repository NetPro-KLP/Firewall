#ifndef __REQUEST_H__
#define __REQUEST_H__

#include "klp_socket.h"

#include "hash.h"
int SendHeader(klp_socket_t sock_fd, char *code, void *data);
int SendExpData(klp_socket_t sock_fd, klp_flow *data);
int SenderExp(hash *data_table, char *code);

#endif
