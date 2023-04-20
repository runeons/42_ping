#include "ft_ping.h"
#include "errno.h"

#define MAX_IP_LEN          16
#define MAX_HOSTNAME_LEN    100
#define TTL_VALUE           64
#define ICMP_PACKET_LEN     64
#define ICMP_PAYLOAD_LEN    56
#define SLEEP_WAIT          1000000


typedef struct  s_packet
{
	struct icmphdr  h;
	char            payload[ICMP_PAYLOAD_LEN];
}               t_packet;

typedef struct  s_data
{
    char        *param;
    char        flags[2];
    char        *ip;
    char        *hostname;
    int         socket;
    struct sockaddr_in address;
    int         loop;
    t_packet    packet;
    int         sequence;
}					t_data;

void    exit_error(char *msg)
{
    dprintf(2, "%s\n", msg);
    free_all_malloc();
    exit(1);
}

void init_data(t_data *dt)
{
    dt->param = "";
    dt->flags[0] = ' ';
    dt->flags[1] = ' ';
    if ((dt->ip = (char *)mmalloc(sizeof(char) * MAX_IP_LEN + 1)) == NULL)
        exit_error("Malloc error.");
    dt->hostname = "";
    dt->socket = 0;
    dt->address.sin_family = AF_INET;
    dt->address.sin_port = 0;
    dt->address.sin_addr.s_addr = INADDR_ANY;
    dt->loop = 1;
    // dt->packet =  
    dt->sequence = 0;
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
    printf("dt.loop : %d\n", dt.loop);
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
    char    host[MAX_HOSTNAME_LEN]; // what is the maximum size I can use ?
    int     r;


    if (inet_aton(dt->ip, &(dt->address.sin_addr)) <= 0)
        exit_error("address error: Invalid IPv4 address.");
    r = getnameinfo((struct sockaddr*)&dt->address, sizeof(dt->address), host, sizeof(host), NULL, 0, 0);
    if (r != 0)
        exit_error("address error: The hostname could not be resolved.");
    else
    {

        dt->hostname = ft_strdup(host);
        if (dt->hostname == NULL)
        {
    
            exit_error("Memory error: Malloc failure.");
        }
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

        dt->ip = ft_strdup(inet_ntoa(((struct sockaddr_in *)tmp->ai_addr)->sin_addr)); // need to free if many ?
        if (dt->ip == NULL)
            exit_error("Memory error: Malloc failure.");
        tmp = tmp->ai_next;
    }
    freeaddrinfo(res);
}

// int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
// int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

void open_socket(t_data *dt)
{
    int ttl_value;
    int r;

    r = 0;
    ttl_value = TTL_VALUE;
    dt->socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (dt->socket < 0)
    {

        exit_error("socket error: Check that you have the correct rights.");
    }

    struct timeval tv_out;
	tv_out.tv_sec = 1;
	tv_out.tv_usec = 0;

    r = setsockopt(dt->socket, IPPROTO_IP, IP_TTL, &ttl_value, sizeof(ttl_value)); // IPPROTO_IP or SOL_IP orSOL_SOCKET ?
    if (r != 0)
    {

        dprintf(2, "setsockopt1: %s\n", strerror(-1));
        exit_error("socket error: Exiting program.");
    }
    r = setsockopt(dt->socket, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));
    if (r != 0)
    {

        dprintf(2, "setsockopt2: %s\n", strerror(-1));
        exit_error("socket error: Exiting program.");
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

void print_icmp_packet(t_data *dt)
{
    printf("dt->packet.payload: %s\n", dt->packet.payload);
    printf("sizeof(dt->packet.payload): %lu\n", sizeof(dt->packet.payload));
    printf("sizeof(dt->packet): %lu\n", sizeof(dt->packet));
    printf("sizeof(dt->sequence): %lu\n", sizeof(dt->sequence));
    printf("dt->sequence: %d\n", dt->sequence);
}

void craft_icmp_packet(t_data *dt)
{
    int i;

    i = 0;
    ft_bzero(&dt->packet, sizeof(dt->packet));
    while (i < ICMP_PAYLOAD_LEN)
    {
		dt->packet.payload[i] = 'A';
        i++;
    }
    // dt->packet.payload[ICMP_PAYLOAD_LEN - 1] = '\0';
    dt->sequence++;

    dt->packet.h.type = ICMP_ECHO;
    dt->packet.h.code = 0;
    dt->packet.h.un.echo.id = getpid();
    dt->packet.h.un.echo.sequence = dt->sequence;
    dt->packet.h.un.gateway = 0;
    dt->packet.h.un.frag.mtu = 0;
    dt->packet.h.checksum = 0;
}

// ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);

void send_and_receive_packet(t_data *dt)
{
    int r;
    struct msghdr buf;
    
    r = 0;
    r = sendto(dt->socket, &dt->packet, sizeof(dt->packet), 0, (struct sockaddr*)&dt->address, sizeof(dt->address));
    printf("errno: %d\n", errno);
    printf(TEST);
    if (r <= 0)
    {
        printf(TEST);
        printf("r: %d\n", r);
        dprintf(2, "packet sending failure: %s\n", strerror(r));
    }
    else
    {
        printf(TEST);
        r = recvmsg(dt->socket, &buf, 0);
        printf(TEST);
        if (r < 0)
            dprintf(2, "packet receiving failure: %s\n", strerror(r));
        else
        {
            printf("Packet received\n");
            printf("buf.msg_name: %s\n", (char *)buf.msg_name);
            printf("buf.msg_flags: %d\n", (buf.msg_flags));
            printf("buf.msg_iovlen: %zu\n", buf.msg_iovlen);
        }
    }
}

// ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);

// void receive_packet(t_data *dt)
// {

// }

void send_ping(t_data *dt)
{
    // while (1)
    // {
    craft_icmp_packet(dt);
    usleep(SLEEP_WAIT);
    send_and_receive_packet(dt);
    // receive_packet(dt);
    // }

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
    send_ping(&dt);
    // end
    free_all_malloc();
    // while(1);
    return (0);
}