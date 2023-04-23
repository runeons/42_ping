#include "ft_ping.h"

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
    dt->bytes = 0;
    dt->ttl = TTL_VALUE;
    dt->ms = 0;
    dt->sent_nb = 0;
    dt->recv_nb = 0;
    dt->init_tv.tv_sec = 0;
    dt->init_tv.tv_usec = 0;
    // tv and tz
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
    printf("dt.bytes : %d\n", dt.bytes);
    printf("dt.ttl : %d\n", dt.ttl);
    printf("dt.ms : %f\n", dt.ms);
    printf("dt.init_tv.tv_sec : %ld\n", dt.init_tv.tv_sec);
    printf("dt.init_tv.tv_usec : %ld\n", dt.init_tv.tv_usec);
    // printf("dt.current_tv.tv_sec : %ld\n", dt.current_tv.tv_sec);
    // printf("dt.current_tv.tv_usec : %ld\n", dt.current_tv.tv_usec);
}

void    print_addrinfo(struct addrinfo *ai)
{
    printf("\n");
    printf("ai_family: %d\n", ai->ai_family);
    printf("ai_socktype: %d\n", ai->ai_socktype);
    printf("ai_addr: %s\n", inet_ntoa(((struct sockaddr_in *)ai->ai_addr)->sin_addr));
}
