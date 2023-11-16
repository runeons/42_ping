#include "ft_ping.h"
#include "options.h"

int g_main_loop = 1;

void    end_loop(int err)
{
    (void)err;
    g_main_loop = 0;
}

void    add_destination(t_data *dt, char *curr_arg)
{
    if (dt)
    {
        dt->input_dest = curr_arg;
    }
}

void    option_h()
{
    display_help();
    free_all_malloc();
    exit(0);
}

int main(int ac, char **av)
{
    t_data          dt;
    t_parsed_cmd    parsed_cmd;

    // initial parsing
    if (ac < 2)
        exit_error("usage error: Destination address required. ft_ping -h for help.\n");
    parsed_cmd = parse_options(ac, av);
    debug_activated_options(parsed_cmd.act_options);
    // initial execution and second step parsing
    if (is_activated_option(parsed_cmd.act_options, 'h'))
        option_h();
    init_data(&dt);
    if (ft_lst_size(parsed_cmd.not_options) != 1)
        exit_error("usage error: Destination required and only one.\n");
    else
        add_destination(&dt, parsed_cmd.not_options->content);
    resolve_address(&dt);
    resolve_hostname(&dt);
    // print_data(dt);
    // second step execution
    open_socket(&dt);
    signal(SIGINT, end_loop);
    // print_data(dt);
    print_init_ping(&dt);
    if (gettimeofday(&dt.init_tv, &dt.tz) != 0)
        exit_error("time error: Cannot retrieve time\n");
    while (g_main_loop)
        ping(&dt);
    // end of execution
    print_statistics(&dt);
    close(dt.socket);
    free_all_malloc();
    // while(1);
    return (0);
}