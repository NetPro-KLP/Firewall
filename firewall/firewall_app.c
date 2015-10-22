#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "netpro_protocol.h"
#include "code.h"

#define BUFSIZE	1024

#define ADDR 	"172.16.101.163"
#define PORT 	30000


char* itoa(int val, int base){
	
	static char buf[32] = {0};
	
	int i = 30;
	
	while (val && i)
	{
		buf[i] = "0123456789abcdef"[val % base];
		--i;
		val /= base;
	}		
	
	return &buf[i+1];
}

int main(void)
{
	int client_fd;

	struct sockaddr_in	server_addr;

	char buf[BUFSIZE + 8];
	unsigned int code = 101000;
	
	char payload[] = {(char)(code & 0xff000000), (char)(code & 0x00ff0000), 
		(char)(code & 0x0000ff00), (char)(code & 0x000000ff),
						'r','e','q','?', 
						'0','0','0','0','0','0','0','0','0','0',
						'0','0','0','0','0','0','0','0','0',
						'0','0','0','0','1', 'c','o','n','t','e','n','t','\0'};

	client_fd = socket(PF_INET, SOCK_STREAM, 0);
	if(client_fd == -1)
	{
		char * msg = "socket() is failed\n";
		write(STDOUT_FILENO, msg, strlen(msg));
		return -1;
	}

	memset(&server_addr, 0, sizeof(server_addr));
	
	server_addr.sin_family		= AF_INET;
	server_addr.sin_port		= htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr(ADDR);

	if(connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
	{
		char * msg = "connect() is failed\n";
		write(STDOUT_FILENO, msg, strlen(msg));
		return -1;
	}

	/* send payload size
		send payload payload is like this.
		code    | req or res | firewall number | content 
		4bytes	    4bytes		24 bytes			n bytes(variable)
	*/



	close(client_fd);
	
	return 0;
}
