#include "ft_ping.h"

void init_data(t_data *dt)
{
    dt->input_dest = "";
    dt->act_options = NULL;
    // if ((dt->resolved_address = (char *)mmalloc(sizeof(char) * MAX_IP_LEN + 1)) == NULL)
        // exit_error("Malloc error.\n");
    dt->resolved_address = NULL;
    dt->resolved_hostname = "";
    dt->address.sin_family = AF_INET;
    dt->address.sin_port = 0;
    dt->address.sin_addr.s_addr = INADDR_ANY;
    dt->socket = 0;

    dt->loop = 1;
    dt->sequence = 0;
    // address not initialized ?
    dt->bytes = 0;
    dt->ttl = TTL_VALUE;
    dt->ms = 0;
    dt->sent_nb = 0;
    dt->recv_nb = 0;
    dt->init_tv.tv_sec = 0;
    dt->init_tv.tv_usec = 0;
    ft_bzero(dt->v_buf, sizeof(dt->v_buf));
    // tv and tz
}

void print_data(t_data dt)
{
    printf("\n");
    printf("dt.input_dest : %s\n", dt.input_dest);
    printf("dt.resolved_address : %s\n", dt.resolved_address);
    printf("dt.resolved_hostname : %s\n", dt.resolved_hostname);
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
    printf("dt.v_buf : %s\n", dt.v_buf);
    // printf("dt.current_tv.tv_sec : %ld\n", dt.current_tv.tv_sec);
    // printf("dt.current_tv.tv_usec : %ld\n", dt.current_tv.tv_usec);
}
