#ifndef PING_DATA_H
# define PING_DATA_H

# include "utils_constants.h"

struct icmphdr
{
    unsigned char type;                /* message type */
    unsigned char code;                /* type sub-code */
    unsigned short int checksum;
    union
    {
        struct
        {
            unsigned short int        id;
            unsigned short int        sequence;
        } echo;                        /* echo datagram */
        unsigned int        gateway;        /* gateway address */
        struct
        {
            unsigned short int        __unused;
            unsigned short int        mtu;
        } frag;                        /* path mtu discovery */
    } un;
};

typedef struct  s_packet
{
	struct icmphdr  h;
	char            payload[ICMP_PAYLOAD_LEN];
}               t_packet;

typedef struct  s_ping_seq
{
    int             bytes;      // ping recv bytes nb             // display seq
    int             icmp_seq;   // icmp cpunt // icmp_seq         // display seq
    int             ttl;        // setup at start                  // display seq
    struct timeval  send_tv;    // icmp stats    // display seq
    struct timeval  receive_tv; // icmp stats      // display seq
}               t_ping_seq;

typedef struct  s_ping_stats
{
    int sent_nb;  // icmp count    // display end stats
    int recv_nb;  // icmp count    // display end stats
}               t_ping_stats;

typedef struct  s_options_params
{
    char        p_payload[ICMP_PAYLOAD_LEN];
    int         seq_interval_us;
}               t_options_params;

typedef struct  s_data
{
    char                *input_dest;
    t_lst               *act_options;
    char                *resolved_address;
    char                *resolved_hostname;
    int                 socket;
    struct sockaddr_in  address;
    t_packet            packet;     // icmp                           // display_icmp_seq_init
    struct timeval      tz;         // icmp stats   // display seq // display end stats // display end stats
    struct timeval      init_tv;    // icmp stats   // display end stats // display end stats
    t_ping_seq          one_seq;
    t_ping_stats        end_stats;
    t_options_params    options_params;
}					t_data;

#endif

