#include "ping_functions.h"

    // struct iovec {                    /* Scatter/gather array items */
    //     void  *iov_base;              /* Starting address */
    //     size_t iov_len;               /* Number of bytes to transfer */
    // };

    // struct msghdr {
    //     void         *msg_name;       /* optional address */
    //     socklen_t     msg_namelen;    /* size of address */
    //     struct iovec *msg_iov;        /* scatter/gather array */
    //     size_t        msg_iovlen;     /* # elements in msg_iov */
    //     void         *msg_control;    /* ancillary data, see below */
    //     size_t        msg_controllen; /* ancillary data buffer len */
    //     int           msg_flags;      /* flags on received message */
    // };

static void    save_packet(t_data *dt)
{
    ft_bzero(&dt->one_seq.final_packet, sizeof(dt->one_seq.final_packet));
    dt->one_seq.final_packet.ip = (struct iphdr *)dt->one_seq.r_packet;
    dt->one_seq.final_packet.icmp = (struct icmphdr *)(dt->one_seq.r_packet + IP_HEADER_LEN);
    dt->one_seq.final_packet.payload = (char *)(dt->one_seq.r_packet + IP_HEADER_LEN + ICMP_HEADER_LEN);
}

static void    save_time(t_data *dt)
{
    int *time;

    if (!(time = mmalloc(sizeof(int))))
        exit_error("ping: malloc failure.");
    *time = (dt->one_seq.receive_tv.tv_sec - dt->one_seq.send_tv.tv_sec) * 1000000 + dt->one_seq.receive_tv.tv_usec - dt->one_seq.send_tv.tv_usec;
    dt->one_seq.time = *time;
    ft_lst_add_node_back(&dt->end_stats.times, ft_lst_create_node(time));
}

static void    handle_reply(t_data *dt, struct msghdr *msgh)
{
    if (gettimeofday(&dt->one_seq.receive_tv, &dt->tz) != 0)
        exit_error("ping: cannot retrieve time\n");
    save_time(dt);
    save_packet(dt);
    dt->one_seq.bytes = sizeof(*msgh) + ICMP_HEADER_LEN;
    if (dt->one_seq.final_packet.icmp->type == ICMP_ECHO_REPLY)
    {
        display_ping_OK(dt);
        dt->end_stats.recv_nb++;
    }
    else
    {
        if (dt->one_seq.final_packet.icmp->type == ICMP_ERR_UNREACHABLE)
            display_ping_error(dt, "Destination Host Unreachable");
        else if (dt->one_seq.final_packet.icmp->type == ICMP_ERR_TIME_EXCEEDED)
            display_ping_error(dt, "Time to live exceeded");
        else
            warning_error(C_G_BLUE"UNHANDLED type %d"C_RES"\n", dt->one_seq.final_packet.icmp->type);
    }
    debug_packet(&(dt->one_seq.final_packet));
}

char    *addrinfo_to_str(struct addrinfo *addr)
{
    struct addrinfo     *tmp;
    tmp = addr;

    char ip_str[MAX_IP_LEN];

    if (inet_ntop(tmp->ai_family, &((struct sockaddr_in *)tmp->ai_addr)->sin_addr, ip_str, sizeof(ip_str)) == NULL)
        exit_error("ping: address error: Conversion from network to presentation format failed.\n");

    return (ft_strdup(ip_str));
}

char    *sockaddr_in_to_str(struct sockaddr_in *addr)
{
    char ip_str[MAX_IP_LEN];

    if (inet_ntop(addr->sin_family, &addr->sin_addr, ip_str, sizeof(ip_str)) == NULL)
        exit_error("ping: address error: Conversion from network to presentation format failed.\n");

    return (ft_strdup(ip_str));
}

static void    send_icmp_and_receive_packet(t_data *dt)    
{
    int                     r = 0;	
    struct msghdr           msgh;
    
    memset(&msgh, 0, sizeof(msgh));
    if (gettimeofday(&dt->one_seq.send_tv, &dt->tz) != 0)
        exit_error("ping: cannot retrieve time\n");
    r = sendto(dt->socket, &dt->crafted_icmp, sizeof(dt->crafted_icmp), 0, (struct sockaddr*)&dt->address, sizeof(dt->address));
    if (r <= 0)
        warning_error(C_G_RED"packet sending failure:"C_RES"\n");
    else if (r != sizeof(dt->crafted_icmp))
        warning_error(C_G_RED"packet not entirely sent:"C_RES"\n");
    else
    {
        // warning_error(C_G_BLUE"packet apparently sent:"C_RES"\n");
        dt->end_stats.sent_nb++;
        init_recv_msgh(&msgh, dt->one_seq.r_packet);
        r = recvmsg(dt->socket, &msgh, 0);
        if (r >= 0)
            handle_reply(dt, &msgh);
        // else
            // warning_error(C_G_RED"packet receiving failure:"C_RES"\n");
    }
}

int    end_max_count(t_data *dt)
{
    if (dt->options_params.count > 0 && dt->end_stats.sent_nb >= dt->options_params.count)
    {
        if (dt->end_stats.recv_nb >= dt->options_params.count)
            return 1;
        else
            return 1; // TO DO jump to recv only without sending any other packet
    }
    return 0;
}

int     end_timeout(t_data *dt)
{
    struct timeval      curr_tv; 
    
    if (dt->options_params.w_timeout > 0)
    {
        if (gettimeofday(&curr_tv, &dt->tz) != 0)
            exit_error("ping: cannot retrieve time\n");
        if ((curr_tv.tv_sec - dt->init_tv.tv_sec) >= dt->options_params.w_timeout)
            return 1;
    }
    return 0;
}

void ping_sequence(t_data *dt)
{
    if (end_max_count(dt) || end_timeout(dt))
    {
        g_ping = 0;
        return ;
    }
    craft_icmp(dt);
    debug_crafted_icmp(&dt->crafted_icmp);
    usleep(dt->options_params.seq_interval_us);
    send_icmp_and_receive_packet(dt);
}