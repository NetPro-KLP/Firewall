
#ifndef __KLP_PROTOCOL_H__
#define __KLP_PROTOCOL_H__

//// code


//// method
#define REQ	"req"
#define RES	"res"

// protocol header
typedef struct klp_header
{
	unsigned int payload_size;
} klp_header;

// protocol payload
typedef struct klp_payload
{
	unsigned int 	code;
	char 			*method;
	unsigned int 	firewall_id;
	unsigned int 	start_idx;
	unsigned int 	end_idx;
	unsigned int 	current_idx;
	unsigned int 	contents_size;
	char			*data;
} klp_payload;

// key value of klp_flow 
typedef struct klp_key
{
    unsigned int saddr;  // source address
    short int src;  // soruce port number
    
    unsigned int daddr;  // destination address
    short int dest; // destination port number
    
    short int protocol; // protocol
    
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
    int starttime;  // start time
    int endtime;    // end tine
    
    
} klp_flow;

#endif
