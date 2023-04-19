#include "ft_ping.h"

#define MAX_IP_LEN          16
#define MAX_HOSTNAME_LEN    100

typedef struct s_data
{
    char        *param;
    char        flags[2];
    char        *ip;
    char        *hostname;
    int         socket;
    struct sockaddr_in address;
}					t_data;

void    exit_error(char *msg)
{
    dprintf(2, "%s\n", msg);
    exit(1);
}

void init_data(t_data *dt)
{
    dt->param = "";
    dt->flags[0] = ' ';
    dt->flags[1] = ' ';
    if ((dt->ip = (char *)malloc(sizeof(char) * 16 + 1)) == NULL)
        exit_error("Malloc error.");
    dt->hostname = "";
    dt->socket = 0;
    dt->address.sin_family = AF_INET;
    dt->address.sin_port = 80;
    dt->address.sin_addr.s_addr = INADDR_ANY;
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
    dt->ip = av[1];
    dt->hostname = av[1];
}

void check_address(t_data *dt)
{
    char    host[10000]; // what is the maximum size I can use ?
    int     r;

    if (inet_aton(dt->ip, &(dt->address.sin_addr)) <= 0)
        exit_error("address error: Invalid IPv4 address.");
    r = getnameinfo((struct sockaddr*)&dt->address, sizeof(dt->address), host, sizeof(host), NULL, 0, 0);
    if (r != 0)
        exit_error("address error: The hostname could not be resolved.");
    else
    {
        dt->hostname = host; // need strdup ?
    }
}

// int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);

void    init_hints(struct addrinfo *hints)
{
    memset(hints, 0, sizeof(*hints));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_RAW;
	hints->ai_protocol = IPPROTO_ICMP;
}

void    print_addrinfo(struct addrinfo *ai)
{
    printf("\n");
    printf("ai_family: %d\n", ai->ai_family);
    printf("ai_socktype: %d\n", ai->ai_socktype);
    printf("ai_addr: %s\n", inet_ntoa(((struct sockaddr_in *)ai->ai_addr)->sin_addr));
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
        dt->ip = ft_strdup(inet_ntoa(((struct sockaddr_in *)tmp->ai_addr)->sin_addr));
        tmp = tmp->ai_next;
    }
    freeaddrinfo(res);
}

void open_socket(t_data *dt)
{
    dt->socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (dt->socket < 0)
    {
        exit_error("socket error: Check that you have the correct rights.");
    }
}

void    print_statistics()
{
    return;
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
    // ping
    print_data(dt);
    return (0);
}