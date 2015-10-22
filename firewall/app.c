#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "myprotocol.h"

#define DEVICE_FILE_NAME "/dev/drop"

int main(void)
{
	int dev;

	printf("one start\n");

	dev = open(DEVICE_FILE_NAME, O_RDWR|O_NDELAY);

	printf("open end\n");
	printf("%d\n", dev);
	if(dev < 0)
	{
		printf("open error!\n");
		return -1;
	}
	struct myprotocol msg;
	msg.cmd = ADD_IP;
	strcpy(msg.addr, "125.209.222.142");
	write(dev, (char*)&msg, sizeof(msg));
	close(dev);
}
