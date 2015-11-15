#ifndef __REQUEST_H__
#define __REQUEST_H__

#include "ksocket.h"

#include "hash.h"
#include "trie.h"
int SendHeader(ksocket_t sock_fd, char *code, void *data);
int SendExpData(ksocket_t sock_fd, klp_flow *data);
int SenderExp(hash *data_table, char *code);

int ReceiveIniData(ksocket_t sock_fd, node **root);
#endif
