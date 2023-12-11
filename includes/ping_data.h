#ifndef PING_DATA_H
# define PING_DATA_H

# include "utils_constants.h"

// struct icmphdr
// {
//     unsigned char type;                /* message type */
//     unsigned char code;                /* type sub-code */
//     unsigned short int checksum;
//     union
//     {
//         struct
//         {
//             unsigned short int        id;
//             unsigned short int        sequence;
//         } echo;                        /* echo datagram */
//         unsigned int        gateway;        /* gateway address */
//         struct
//         {
//             unsigned short int        __unused;
//             unsigned short int        mtu;
//         } frag;                        /* path mtu discovery */
//     } un;
// };

extern int g_ping;

typedef struct  s_packet
{
    struct iphdr    *ip;
    struct icmphdr  *icmp;
    char            *payload;
}               t_packet;

typedef struct  s_icmp
{
	struct icmphdr  h;
	char            timestamp[ICMP_TIMESTAMP_LEN];
	char            payload[ICMP_PAYLOAD_LEN];
}               t_icmp;

typedef struct  s_ping_seq
{
    int             bytes;
    int             icmp_seq_count;
    int             ttl;
    struct timeval  send_tv;
    struct timeval  receive_tv;
    char            r_packet[IP_TOTAL_LEN];
    t_packet        final_packet;
    int             time;
}               t_ping_seq;

typedef struct  s_ping_stats
{
    int sent_nb;
    int recv_nb;
    t_lst  *times;
}               t_ping_stats;

typedef struct  s_options_params
{
    char        p_payload[ICMP_PAYLOAD_LEN];
    int         seq_interval_us;
    int         count;
    int         v;
    int         w_timeout;
}               t_options_params;

typedef struct  s_data
{
    char                *input_dest;
    t_lst               *act_options;
    char                *resolved_address;
    char                *resolved_hostname;
    int                 socket;
    struct sockaddr_in  address;
    t_icmp              crafted_icmp;
    struct timeval      tz;
    struct timeval      init_tv;
    t_ping_seq          one_seq;
    t_ping_stats        end_stats;
    t_options_params    options_params;
}					t_data;

#endif

