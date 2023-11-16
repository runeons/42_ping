#include "ft_ping.h"

void resolve_hostname(t_data *dt) // useful only when input_dest is ip address (vs. hostname)
{
    char    host[MAX_HOSTNAME_LEN]; // what is the maximum size I can use ?

    ft_bzero(host, MAX_HOSTNAME_LEN);
    if (inet_aton(dt->resolved_address, &(dt->address.sin_addr)) <= 0)
        exit_error("address error: Invalid IPv4 address.\n");
    if (getnameinfo((struct sockaddr*)&dt->address, sizeof(dt->address), host, sizeof(host), NULL, 0, 0) != 0)
        exit_error("address error: The hostname could not be resolved.\n");
    else
    {
        dt->resolved_hostname = ft_strdup(host);
        if (dt->resolved_hostname == NULL)
            exit_error("Memory error: Malloc failure.\n");
    }
    printf(C_B_RED"dt->resolved_hostname %s"C_RES"\n", dt->resolved_hostname); // 
}

// void    init_hints(struct addrinfo *hints)
// {
//     memset(hints, 0, sizeof(*hints));
// 	hints->ai_family = AF_INET;
// 	hints->ai_socktype = SOCK_RAW;
// 	hints->ai_protocol = IPPROTO_ICMP;
// }

void resolve_address(t_data *dt)
{
    int                 r;
    // struct addrinfo     hints;
    struct addrinfo     *res;
    struct addrinfo     *tmp;

    // init_hints(&hints);
    // r = getaddrinfo(dt->input_dest, NULL, &hints, &res);
    r = getaddrinfo(dt->input_dest, NULL, NULL, &res);
    debug_addrinfo(res);
    if (r != 0)
        exit_error("address error: The ip address could not be resolved. getaddrinfo: %s\n", gai_strerror(r));
    tmp = res;
    while (tmp != NULL)
    {
        dt->resolved_address = ft_strdup(inet_ntoa(((struct sockaddr_in *)tmp->ai_addr)->sin_addr)); // need to free if many ?
        if (dt->resolved_address == NULL)
            exit_error("Memory error: Malloc failure.\n");
        tmp = tmp->ai_next;
    }
    printf(C_B_RED"dt->resolved_address %s"C_RES"\n", dt->resolved_address);
    freeaddrinfo(res);
}

