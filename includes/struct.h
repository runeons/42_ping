#ifndef STRUCT_H
# define STRUCT_H

#define MAX_IP_LEN          16
#define MAX_HOSTNAME_LEN    100
#define TTL_VALUE           64
#define ICMP_PACKET_LEN     64
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
    char        flags[2];
    char        *ip;
    char        *hostname;
    int         socket;
    struct sockaddr_in address;
    int         loop;
    t_packet    packet;
    int         sequence;
}					t_data;

#endif

