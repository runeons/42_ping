#include "ft_ping.h"

typedef struct s_data
{
    char    *param;
    char    flags[2];
    char    *ip;
    char    *hostname;
}					t_data;

void    exit_error(char *msg)
{
    dprintf(2, "%s\n", msg);
    exit(1);
}

void    add_flag(t_data *dt, char flag)
{
    if (ft_strchr(dt->flags, flag))
        return;
    if (dt->flags[0] == ' ')
        dt->flags[0] = flag;
    else if (dt->flags[1] == ' ')
        dt->flags[1] = flag;
    else
        exit_error("Unknow error - too many flags");
}

void    parse_params(int ac, char **av, t_data *dt)
{
    int i;
    char *allowed_flags;

    i = 1;
    allowed_flags = "hv";
    while (i < ac && av && av[i])
    {
        if (ft_strlen(av[i]) >= 2 && av[i][0] == '-')
        {
            if (ft_strlen(av[i]) == 2 && ft_strchr(allowed_flags, av[i][1]))
                add_flag(dt, av[i][1]);
            else
            {
                dprintf(2, "invalid option -- '%s'\n", av[i]);
                exit(1);
            }
        }
        else
        {
            if (ft_strlen(dt->param) == 0)
                dt->param = av[i];
            else
            {
                dprintf(2, "usage error: Only one ip/hostname required, unrecognised pattern -- '%s'\n", av[i]);
                exit(1);
            }
        }
        i++;
    }

}

void init_data(t_data *dt)
{
    dt->param = "";
    dt->flags[0] = ' ';
    dt->flags[1] = ' ';
    dt->ip = "";
    dt->hostname = "";
}

void print_data(t_data dt)
{
    printf("dt.param : %s\n", dt.param);
    printf("dt.flags : %c\n", dt.flags[0]);
    printf("dt.flags : %c\n", dt.flags[1]);
    printf("dt.ip : %s\n", dt.ip);
    printf("dt.hostname : %s\n", dt.hostname);
}

int main(int ac, char **av)
{
    t_data dt;

    if (ac < 2)
        exit_error("usage error: Destination address required");
    init_data(&dt);
    parse_params(ac, av, &dt);
    print_data(dt);
    printf("av[1]: %s\n", av[1]);
    return (0);
}