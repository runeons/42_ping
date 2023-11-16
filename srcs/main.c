#include "ft_ping.h"
#include "options.h"

int g_in_progress = 1;

void    handle_sigint(int err)
{
    (void)err;
    g_in_progress = 0;
}

void    add_destination(t_data *dt, char *curr_arg)
{
    if (dt)
        dt->input_dest = curr_arg;
}

void    option_h()
{
    display_help();
    free_all_malloc();
    exit(0);
}

// void    parse(t_parsed_cmd *parsed_cmd)
// {

// }

// void    initialise(t_data *dt)
// {

// }


int main(int ac, char **av)
{
    t_data          dt;
    t_parsed_cmd    parsed_cmd;

    // initial parsing
    if (ac < 2)
        exit_error("usage error: Destination address required. ft_ping -h for help.\n");
    parsed_cmd = parse_options(ac, av);
    debug_activated_options(parsed_cmd.act_options);
    if (is_activated_option(parsed_cmd.act_options, 'h'))
        option_h();
    init_data(&dt);
    if (ft_lst_size(parsed_cmd.not_options) != 1)
        exit_error("usage error: Destination required and only one.\n");
    else
        add_destination(&dt, parsed_cmd.not_options->content);
    // socket execution
    resolve_address(&dt);
    resolve_hostname(&dt);
    open_socket(&dt);
    set_socket_options(dt.socket);
    signal(SIGINT, handle_sigint);
    display_ping_init(&dt);
    if (gettimeofday(&dt.init_tv, &dt.tz) != 0)
        exit_error("time error: Cannot retrieve time\n");
    while (g_in_progress)
        ping(&dt);
    // end of execution
    display_ping_end_stats(&dt);
    close(dt.socket);
    free_all_malloc();
    printf(C_B_RED"[DEBUG] END"C_RES"\n");
    return (0);
}