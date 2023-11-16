#ifndef PING_FUNCTIONS_H
# define PING_FUNCTIONS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <signal.h>
# include <sys/time.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netdb.h>
// # include <netinet/ip_icmp.h> # that includes ICMP_ECHO and icmphdr
# include <libft.h>
# include <utils_colors.h>
# include <utils_constants.h>
# include <ping_data.h>
# include <errno.h>

//  ping.c
void            craft_icmp_payload(t_data *dt);
unsigned short  checksum(void *b, int len);
void            receive_packet(t_data *dt);
void            ping_sequence(t_data *dt);

//  socket.c
void            resolve_hostname(t_data *dt);
void            resolve_address(t_data *dt);
void            open_socket(t_data *dt);
void            set_socket_options(int socket);

//  init_data.c
void            init_data(t_data *dt);

//  display.c
void            display_ping_init(t_data *dt);
void            display_ping_sequence(t_data *dt);
void            display_ping_end_stats(t_data *dt);

//  utils_debug.c
void            debug_addrinfo(struct addrinfo *ai);
void            debug_icmp_packet(t_data *dt);
void            debug_buf(struct msghdr msg);

#endif

