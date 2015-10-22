#ifndef __NETPRO_PROTOCOL_H__
#define __NETPRO_PROTOCOL_H__

typedef struct netpro_header
{
	unsigned int header_size;
	unsigned int payload_size;
} netpro_header;

typedef struct netpro_payload
{
	unsigned int code;
	char method[4];
	unsigned int firewall_id;
} netpro_payload;

typedef struct netpro_contents
{
	unsigned int start_index;
	unsigned int end_index;
	unsigned int current_index;
	unsigned int contents_size;
	char* data;
} netpro_contents;

typedef struct netpro_flow
{
	unsigned int source_ip;
	unsigned short int source_port;
	unsigned int destination_ip;
	unsigned short int destination_port;
	unsigned short int protocol;
	unsigned int start_time;
	unsigned int end_time;
	unsigned int type;
	unsigned int count;
	unsigned int isnormal;
	unsigned int reserved_1;
	unsigned int reserved_2;
	unsigned int reserved_3;
} netpro_flow;


#endif