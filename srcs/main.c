#include "ft_ping.h"

int g_main_loop = 1;

void    exit_error(char *msg)
{
    dprintf(2, "%s\n", msg);
    free_all_malloc();
    exit(1);
}

void    end_loop(int err)
{
    (void)err;
    g_main_loop = 0;
}

int main(int ac, char **av)
{
    t_data dt;

    (void)av;
    if (ac < 2)
        exit_error("usage error: Destination address required");
    init_data(&dt);
    parse_params(ac, av, &dt);
    // print_data(dt);
    check_hostname(&dt);
    check_address(&dt);
    open_socket(&dt);
    signal(SIGINT, end_loop);
    // print_data(dt);
    print_init_ping(&dt);
    if (gettimeofday(&dt.init_tv, &dt.tz) != 0)
        exit_error("time error: Cannot retrieve time");
    while (g_main_loop)
        ping(&dt);
    print_statistics(&dt);
    close(dt.socket);
    free_all_malloc();
    // while(1);
    return (0);
}