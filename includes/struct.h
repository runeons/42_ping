#ifndef STRUCT_H
# define STRUCT_H

#define MAX_IP_LEN          16
#define MAX_HOSTNAME_LEN    100
#define TTL_VALUE           64
#define ICMP_PACKET_LEN     84
#define ICMP_PAYLOAD_LEN    56
#define SLEEP_WAIT          1000000

typedef struct  s_packet
{
	struct icmphdr  h;
	char            payload[ICMP_PAYLOAD_LEN];
}               t_packet;

typedef struct  s_data
{
    char        *param;
    int         flag_h;
    int         flag_v;
    // char        flags[2];
    char        *ip; // address
    char        *hostname;
    int         socket;
    struct sockaddr_in address;
    int         loop;
    t_packet    packet;
    int         sequence; // icmp_seq
    int         bytes;
    int         ttl;
    float       ms;
    struct timeval  tz;
    struct timeval  init_tv;
    struct timeval  send_tv;
    struct timeval  receive_tv;
    int         sent_nb;
    int         recv_nb;
    char        v_buf[1024];
}					t_data;

#endif

