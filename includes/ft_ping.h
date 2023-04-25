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
#include <netdb.h>
#include <netinet/ip_icmp.h>

# include <libft.h>
# include <color_shell.h>
# include <struct.h>

void print_icmp_packet(t_data *dt);
void craft_icmp_payload(t_data *dt);
unsigned short checksum(void *b, int len);
void receive_packet(t_data *dt);
void ping(t_data *dt);

void    add_flag(t_data *dt, char flag);
void    parse_params(int ac, char **av, t_data *dt);
void check_address(t_data *dt);
void    init_hints(struct addrinfo *hints);
void check_hostname(t_data *dt);

void open_socket(t_data *dt);

void    exit_error(char *msg);
void init_data(t_data *dt);
void print_data(t_data dt);
void    print_addrinfo(struct addrinfo *ai);
void    end_loop(int err);

void init_data(t_data *dt);
void print_data(t_data dt);
void    print_addrinfo(struct addrinfo *ai);

void    print_statistics(t_data *dt);
void    print_ping(t_data *dt);
void    print_init_ping(t_data *dt);

#endif

