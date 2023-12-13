#include "ping_functions.h"

static int    is_same_id(t_data *dt) // is reply id the same as request id
{
    if (dt->one_seq.final_packet.icmp->un.echo.id == dt->id) // same id in icmp header
        return (1);
    else 
    {
        unsigned char *bytes = (unsigned char *)dt->one_seq.final_packet.icmp;
        struct icmphdr  *icmp_in_payload = (struct icmphdr *)(bytes + IP_HEADER_LEN + ICMP_HEADER_LEN);

        if (icmp_in_payload->un.echo.id == dt->id)  // same id in icmp header in payload (when Unreachable)
            return (1);
        else
            return (0);
    }
}

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

    if (gettimeofday(&dt->one_seq.receive_tv, &dt->tz) != 0)
        exit_error_close(dt->socket, "ping: cannot retrieve time\n");
    if (!(time = mmalloc(sizeof(int))))
        exit_error_close(dt->socket, "ping: malloc failure.");
    *time = (dt->one_seq.receive_tv.tv_sec - dt->one_seq.send_tv.tv_sec) * 1000000 + dt->one_seq.receive_tv.tv_usec - dt->one_seq.send_tv.tv_usec;
    dt->one_seq.time = *time;
    ft_lst_add_node_back(&dt->end_stats.times, ft_lst_create_node(time));
}

static void    handle_reply(t_data *dt, struct msghdr *msgh)
{
    dt->one_seq.bytes = sizeof(*msgh) + ICMP_HEADER_LEN;
    if (dt->one_seq.final_packet.icmp->type == ICMP_ECHO_REPLY)
    {
        save_time(dt);
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
        {
            if (VERBOSE == 1)
                warning_error(C_G_BLUE"Packet type %d"C_RES"\n", dt->one_seq.final_packet.icmp->type);
        }
    }
    debug_packet(&(dt->one_seq.final_packet));
}

static void    receive_packet(t_data *dt)
{
    int                     r = 0;	
    struct msghdr           msgh;
    int                     same_id = 0;
    while (g_ping)
    {
        ft_memset(&msgh, 0, sizeof(msgh));
        init_recv_msgh(&msgh, dt->one_seq.r_packet, dt->socket);
        r = recvmsg(dt->socket, &msgh, 0);
        if (r >= 0)
        {
            save_packet(dt);
            same_id = is_same_id(dt);
            if (same_id == 0)
                continue; // don't handle this reply if not the same id - wait for another reply
            handle_reply(dt, &msgh);
            break; // send new request after handle_reply when same id as request
        }
        else
        {
            if (VERBOSE == 1)
                warning_error(C_G_BLUE"No reply received"C_RES"\n"); // ignore and send new request if no reply received
            break;
        }
    }
}

static void    send_icmp_and_receive_packet(t_data *dt)    
{
    int                     r = 0;	
    
    if (gettimeofday(&dt->one_seq.send_tv, &dt->tz) != 0)
        exit_error_close(dt->socket, "ping: cannot retrieve time\n");
    r = sendto(dt->socket, &dt->crafted_icmp, sizeof(dt->crafted_icmp), 0, (struct sockaddr*)&dt->address, sizeof(dt->address));
    if (r <= 0)
        warning_error(C_G_RED"packet sending failure"C_RES"\n");
    else if (r != sizeof(dt->crafted_icmp))
        warning_error(C_G_RED"packet not completely sent"C_RES"\n");
    else
    {
        dt->end_stats.sent_nb++;
        receive_packet(dt);
    }
}

int    end_max_count(t_data *dt)
{
    if (dt->options_params.count > 0 && dt->end_stats.sent_nb >= dt->options_params.count)
        return (1);
    return (0);
}

int     end_timeout(t_data *dt)
{
    struct timeval      curr_tv; 
    
    if (dt->options_params.w_timeout > 0)
    {
        if (gettimeofday(&curr_tv, &dt->tz) != 0)
            exit_error_close(dt->socket, "ping: cannot retrieve time\n");
        if ((curr_tv.tv_sec - dt->init_tv.tv_sec) >= dt->options_params.w_timeout)
            return (1);
    }
    return (0);
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
    send_icmp_and_receive_packet(dt);
    usleep(dt->options_params.seq_interval_us);
}