#ifndef STRUCT_H
# define STRUCT_H

# include "constants.h"

typedef struct  s_packet
{
	struct icmphdr  h;
	char            payload[ICMP_PAYLOAD_LEN];
}               t_packet;

typedef struct  s_data
{
    char        *input_dest;
    t_lst       *act_options;
    char        *resolved_address;
    char        *resolved_hostname;

    
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

