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
# include <netinet/ip_icmp.h>
# include <netinet/ip.h>
# include <libft.h>
# include <utils_colors.h>
# include <utils_constants.h>
# include <utils_options.h>
# include <ping_data.h>
# include <errno.h>

//  craft.c
void            craft_icmp(t_data *dt);

//  ping.c
void            ping_sequence(t_data *dt);

//  socket.c
void            resolve_hostname(t_data *dt);
void            resolve_address(t_data *dt);
void            open_socket(t_data *dt);
void            set_socket_options(int socket, t_data *dt);

//  options.c
void            init_options_params(t_data *dt);

//  init_data.c
void            init_data(t_data *dt, t_parsed_cmd *parsed_cmd);
void            init_recv_msgh(struct msghdr *msg, char *r_packet, int socket);
//  display.c
void            display_ping_init(t_data *dt);
void            display_ping_OK(t_data *dt);
void            display_ping_error(t_data *dt, char *msg);
void            display_ping_end_stats(t_data *dt);

//  utils_debug.c
void            debug_addrinfo(struct addrinfo *ai);
void            debug_crafted_icmp(t_icmp *crafted_icmp);
void            debug_packet(t_packet *packet);
void            debug_msghdr(struct msghdr msg);
void            debug_time(void *content);

// utils_time.c
int             get_time_min(t_data *dt);
int             get_time_max(t_data *dt);
int             get_time_sum(t_data *dt);
int             get_time_avg(t_data *dt);
int             get_time_std(t_data *dt);

// utils_libft.c
char            *int_to_bin(int n, int len);
void            print_as_hexa(void *data, int size);

#endif
