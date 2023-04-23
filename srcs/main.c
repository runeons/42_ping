#include "ft_ping.h"

void    exit_error(char *msg)
{
    dprintf(2, "%s\n", msg);
    free_all_malloc();
    exit(1);
}

void    quit_program(int err)
{
    print_statistics();
    exit(err);
}

int main(int ac, char **av)
{
    t_data dt;
    // struct timeval  tv;
    // struct timezone tz;

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
    print_init_ping(&dt);
    if (gettimeofday(&dt.init_tv, &dt.tz) != 0)
        exit_error("time error: Cannot retrieve time");
    printf("dt.init_tv.tv_sec: %ld\n", dt.init_tv.tv_sec);
    printf("dt.init_tv.tv_usec: %ld\n", dt.init_tv.tv_usec);
    while (1)
        ping(&dt);
    // end
    free_all_malloc();
    // while(1);
    return (0);
}