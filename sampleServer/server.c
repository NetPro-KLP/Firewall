#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char buf[1024];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(buf, '0', sizeof(buf)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(30000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

    while(1)
    {
        char header[28] = {0, };
        
        unsigned int ip = 0;
        int row_num = 0;
        char cmd[4] = {0,};

        char *tok = 0;
        int len = 0, i;
        
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
        printf("connect : %d\n", connfd);

        memset(buf, 0, 1024);

        /*  Header
            receive format
            firewall address | number of netflow data | CODE    | null
                10 byte      |  10 byte               | 3 byte  | 1byte
            total 24 byte
            receive size : 28 byte include dummy byte
        */
        len = read(connfd, buf, 1024);
        printf("%s\n", buf);

        // convert ip address from ascii to int
        tok = strtok(buf, "|");
        ip = atoi(tok);

        // convert row num from ascii to int
        tok = strtok(0x00, "|");
        row_num = atoi(tok);

        // get command code
        tok = strtok(0x00, "|");
        strcpy(cmd, tok);
        
        /*printf("HEADER\n");
        printf("firewall address : %u\n", ip);
        printf("row_num : %u\n", row_num);
        printf("cmd : %s\n", cmd);
        printf("===================================\n");
*/
        for(i=0; i<row_num; i++)
        {
            unsigned int saddr, daddr;
            unsigned short int src, dst;
            char tcpudp;
            int warn, danger, packet_count, total_byets;
            char starttime[20] = {0, };
            char endtime[20] = {0, };

            memset(buf, 0, 1024);
            /* Payload
                receive format 
                saddr|src|daddr|dst|tcpudp|warn|danger|packet_count|total_bytes|starttime|endtime|NULL
                10     5   10    5    1     10    10         10         10          20       20    1
                total 112
                receive size 120 include dummy byte
            */
            read(connfd, buf, 1024);
            printf("%s\n", buf);
            // convert saddr from ascii to int
            /*tok = strtok(buf, "|");
            saddr = atoi(tok);
            // convert src from ascii to int
            tok = strtok(0x00, "|");
            src = atoi(tok);
            // convert daddr from ascii to int
            tok = strtok(0x00, "|");
            daddr = atoi(tok);
            // convert dst from ascii to int
            tok = strtok(0x00, "|");
            dst = atoi(tok);
            // convert tcpudp from ascii to int
            tok = strtok(0x00, "|");
            tcpudp = tok[0]-'0';
            // convert warn from ascii to int
            tok = strtok(0x00, "|");
            warn = atoi(tok);
            // convert danger from ascii to int
            tok = strtok(0x00, "|");
            danger = atoi(tok);
            // convert packet_count from ascii to int
            tok = strtok(0x00, "|");
            packet_count = atoi(tok);
            // convert total byte from ascii to int
            tok = strtok(0x00, "|");
            // get start time
            total_byets = atoi(tok);
            tok = strtok(0x00, "|");
            strcpy(starttime, tok);
            // get end time
            tok = strtok(0x00, "|");
            strcpy(endtime, tok);

            printf("saddr : %u | src : %u | daddr : %u | dst : %u |\n",
                saddr, src, daddr, dst);
            printf("tcpudp : %d\n", tcpudp);
            printf("warn : %u | danger : %u | packet_count : %u | total_byets : %u \n",
                warn, danger, packet_count, total_byets);
            printf("start time : %s\nend time : %s\n", starttime, endtime);
            printf("===================================\n");*/
        }

        close(connfd);
        sleep(1);
     }
     close(listenfd);
}