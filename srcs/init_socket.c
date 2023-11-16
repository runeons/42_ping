#include "ping_functions.h"

// void    _init_hints(struct addrinfo *hints)
// {
//     memset(hints, 0, sizeof(*hints));
// 	hints->ai_family = AF_INET;
// 	hints->ai_socktype = SOCK_RAW;
// 	hints->ai_protocol = IPPROTO_ICMP;
// }

void resolve_address(t_data *dt) // check that dest exists and resolve address if input == hostname
{
    int                 r;
    // struct addrinfo     hints;
    struct addrinfo     *res;
    struct addrinfo     *tmp;

    // _init_hints(&hints);
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

void resolve_hostname(t_data *dt) // useful only when input_dest is ip address (vs. hostname)
{
    char    host[MAX_HOSTNAME_LEN]; // what is the maximum size I can use ?

    ft_bzero(host, MAX_HOSTNAME_LEN);
    if (inet_aton(dt->resolved_address, &(dt->address.sin_addr)) <= 0)
        exit_error("address error: Invalid IPv4 address.\n");
    // memset(&(dt->address), 0, sizeof(dt->address));
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

void open_socket(t_data *dt)
{
    dt->socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (dt->socket < 0)
        exit_error("socket error: Check that you have the correct rights. %s\n", strerror(dt->socket));
}

void set_socket_options(int socket)
{
    int r           = 0;
    int ttl_value   = TTL_VALUE + 1;
    struct timeval tv_out;
	tv_out.tv_sec = 1;
	tv_out.tv_usec = 0;
    r = setsockopt(socket, IPPROTO_IP, IP_TTL, &ttl_value, sizeof(ttl_value)); // setting TTL option // IPPROTO_IP or SOL_IP or SOL_SOCKET ?
    if (r != 0)
        exit_error("socket error in setting TTL option: Exiting program. %s\n", strerror(r));
    r = setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out)); // setting timeout option
    if (r != 0)
        exit_error("socket error in setting timeout option: Exiting program. %s\n", strerror(r));
}
