#ifndef FT_PING_H
# define FT_PING_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <signal.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
// # include <netinet/ip_icmp.h> # that  includes ICMP_ECHO and icmphdr

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

# include <libft.h>
# include <color_shell.h>
# include <constants.h>
# include <struct.h>
# include <errno.h>

void            craft_icmp_payload(t_data *dt);
unsigned short  checksum(void *b, int len);
void            receive_packet(t_data *dt);
void            ping(t_data *dt);

//  socket.c
void            resolve_hostname(t_data *dt);
void            resolve_address(t_data *dt);
void            open_socket(t_data *dt);
void            set_socket_options(int socket);

void            init_data(t_data *dt);
void            print_data(t_data dt);
void            handle_sigint(int err);

void            display_ping_init(t_data *dt);
void            display_ping_sequence(t_data *dt);
void            display_ping_end_stats(t_data *dt);

void            debug_addrinfo(struct addrinfo *ai);
void            debug_icmp_packet(t_data *dt);
void            debug_buf(struct msghdr msg);

#endif

