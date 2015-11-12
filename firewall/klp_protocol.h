
#ifndef __KLP_PROTOCOL_H__
#define __KLP_PROTOCOL_H__

// key value of klp_flow 
typedef struct klp_key
{
    unsigned int saddr;  // source address
    unsigned short src;  // soruce port number
    unsigned int daddr;  // destination address
    unsigned short dst; // destination port number
    char tcpudp;    //if tcp 0, else udp 1

} klp_key;
// net flow data
typedef struct klp_flow
{
    klp_key key;
    
    int warn;
    int danger;
    int packet_count;   // number of packet

    int totalbytes; // total packet size
    char starttime[20];  // start time
    char endtime[20];    // end tine
    
    
} klp_flow;

#endif
