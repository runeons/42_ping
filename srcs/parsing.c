#include "ft_ping.h"

void    add_flag(t_data *dt, char flag)
{
    if (flag == 'v')
        dt->flag_v = 1;
    else if (flag == 'h')
        dt->flag_h = 1;
    // if (ft_strchr(dt->flags, flag))
    //     return;
    // if (dt->flags[0] == ' ')
    //     dt->flags[0] = flag;
    // else if (dt->flags[1] == ' ')
    //     dt->flags[1] = flag;
    // else
    //     exit_error("Unknow error - too many flags");
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
    dt->ip = av[1];
    dt->hostname = av[1];
}

void check_address(t_data *dt)
{
    char    host[MAX_HOSTNAME_LEN]; // what is the maximum size I can use ?

    ft_bzero(host, MAX_HOSTNAME_LEN);
    if (inet_aton(dt->ip, &(dt->address.sin_addr)) <= 0)
        exit_error("address error: Invalid IPv4 address.");
    if (getnameinfo((struct sockaddr*)&dt->address, sizeof(dt->address), host, sizeof(host), NULL, 0, 0) != 0)
        exit_error("address error: The hostname could not be resolved.");
    else
    {
        dt->hostname = ft_strdup(host);
        if (dt->hostname == NULL)
            exit_error("Memory error: Malloc failure.");
    }
}

void    init_hints(struct addrinfo *hints)
{
    memset(hints, 0, sizeof(*hints));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_RAW;
	hints->ai_protocol = IPPROTO_ICMP;
}

void check_hostname(t_data *dt)
{
    int                 r;
    struct addrinfo     hints;
    struct addrinfo     *res;
    struct addrinfo     *tmp;

    init_hints(&hints);
    r = getaddrinfo(dt->param, NULL, &hints, &res);
    // print_addrinfo(res);
    if (r != 0)
    {
        dprintf(2, "getaddrinfo: %s\n", gai_strerror(r));
        exit_error("address error: The ip address could not be resolved.");
    }
    tmp = res;
    while (tmp != NULL)
    {
        dt->ip = ft_strdup(inet_ntoa(((struct sockaddr_in *)tmp->ai_addr)->sin_addr)); // need to free if many ?
        if (dt->ip == NULL)
            exit_error("Memory error: Malloc failure.");
        tmp = tmp->ai_next;
    }
    freeaddrinfo(res);
}

