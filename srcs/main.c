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