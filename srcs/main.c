#include "ft_ping.h"

void    exit_error(char *msg)
{
    dprintf(2, "%s\n", msg);
    free_all_malloc();
    exit(1);
}

void init_data(t_data *dt)
{
    dt->param = "";
    dt->flags[0] = ' ';
    dt->flags[1] = ' ';
    if ((dt->ip = (char *)mmalloc(sizeof(char) * MAX_IP_LEN + 1)) == NULL)
        exit_error("Malloc error.");
    dt->hostname = "";
    dt->socket = 0;
    dt->address.sin_family = AF_INET;
    dt->address.sin_port = 0;
    dt->address.sin_addr.s_addr = INADDR_ANY;
    dt->loop = 1;
    // dt->packet =  
    dt->sequence = 0;
    // address not initialized ?
}

void print_data(t_data dt)
{
    printf("\n");
    printf("dt.param : %s\n", dt.param);
    printf("dt.flags[0] : %c\n", dt.flags[0]);
    printf("dt.flags[1] : %c\n", dt.flags[1]);
    printf("dt.ip : %s\n", dt.ip);
    printf("dt.hostname : %s\n", dt.hostname);
    printf("dt.socket : %d\n", dt.socket);
    printf("dt.address.sin_family : %hu\n", dt.address.sin_family);
    printf("dt.address.sin_port : %d\n", dt.address.sin_port);
    printf("dt.address.sin_addr.s_addr : %u\n", dt.address.sin_addr.s_addr);
    printf("dt.loop : %d\n", dt.loop);
}


void    print_addrinfo(struct addrinfo *ai)
{
    printf("\n");
    printf("ai_family: %d\n", ai->ai_family);
    printf("ai_socktype: %d\n", ai->ai_socktype);
    printf("ai_addr: %s\n", inet_ntoa(((struct sockaddr_in *)ai->ai_addr)->sin_addr));
}

void    quit_program(int err)
{
    print_statistics();
    exit(err);
}


int main(int ac, char **av)
{
    t_data dt;
    if (ac < 2)
        exit_error("usage error: Destination address required");
    init_data(&dt);
    parse_params(ac, av, &dt);
    print_data(dt);
    check_hostname(&dt);
    check_address(&dt);
    open_socket(&dt);
    signal(SIGINT, quit_program);
    print_data(dt);
    // while (1)
    // {
    usleep(SLEEP_WAIT);
    ping(&dt);
    // }
    // end
    free_all_malloc();
    // while(1);
    return (0);
}