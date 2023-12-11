#include "ping_functions.h"

static void    save_packet(t_data *dt)
{
    ft_bzero(&dt->one_seq.final_packet, sizeof(dt->one_seq.final_packet));
    dt->one_seq.final_packet.ip = (struct iphdr *)dt->one_seq.r_packet;
    dt->one_seq.final_packet.icmp = (struct icmphdr *)(dt->one_seq.r_packet + IP_HEADER_LEN);
    dt->one_seq.final_packet.payload = (char *)(dt->one_seq.r_packet + IP_HEADER_LEN + ICMP_HEADER_LEN);
    if (dt->one_seq.final_packet.icmp->un.echo.id == dt->id)
    {
        ;
        // printf(C_G_RED"SAME PROCESS ID - SUCCESS code 0"C_RES"\n");
    }
    else 
    {
        unsigned char *bytes = (unsigned char *)dt->one_seq.final_packet.icmp;
        struct icmphdr  *icmp_in_payload = (struct icmphdr *)(bytes + IP_HEADER_LEN + ICMP_HEADER_LEN);

        if (icmp_in_payload->un.echo.id == dt->id)
        {
            ;
            // printf(C_G_RED"SAME PROCESS ID - UNREACHABLE"C_RES"\n");
        }
        else
        {
            ;
            // printf(C_G_RED"DIFFERENT PROCESSES"C_RES"\n");
            // printf(C_G_RED"received : %d"C_RES"\n", dt->one_seq.final_packet.icmp->un.echo.id);
            // printf(C_G_RED"dt->id : %d"C_RES"\n", dt->id);

        }
    }
}

static void    save_time(t_data *dt)
{
    int *time;

    if (!(time = mmalloc(sizeof(int))))
        exit_error_close(dt->socket, "ping: malloc failure.");
    *time = (dt->one_seq.receive_tv.tv_sec - dt->one_seq.send_tv.tv_sec) * 1000000 + dt->one_seq.receive_tv.tv_usec - dt->one_seq.send_tv.tv_usec;
    dt->one_seq.time = *time;
    ft_lst_add_node_back(&dt->end_stats.times, ft_lst_create_node(time));
}

static void    handle_reply(t_data *dt, struct msghdr *msgh)
{
    if (gettimeofday(&dt->one_seq.receive_tv, &dt->tz) != 0)
        exit_error_close(dt->socket, "ping: cannot retrieve time\n");
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
            warning_error(C_G_BLUE"Packet type %d"C_RES"\n", dt->one_seq.final_packet.icmp->type);
    }
    debug_packet(&(dt->one_seq.final_packet));
}

static void    send_icmp_and_receive_packet(t_data *dt)    
{
    int                     r = 0;	
    struct msghdr           msgh;
    
    ft_memset(&msgh, 0, sizeof(msgh)); // ADDED FT
    if (gettimeofday(&dt->one_seq.send_tv, &dt->tz) != 0)
        exit_error_close(dt->socket, "ping: cannot retrieve time\n");
    r = sendto(dt->socket, &dt->crafted_icmp, sizeof(dt->crafted_icmp), 0, (struct sockaddr*)&dt->address, sizeof(dt->address));
    if (r <= 0)
        warning_error(C_G_RED"packet sending failure:"C_RES"\n");
    else if (r != sizeof(dt->crafted_icmp))
        warning_error(C_G_RED"packet not entirely sent:"C_RES"\n");
    else
    {
        dt->end_stats.sent_nb++;
        init_recv_msgh(&msgh, dt->one_seq.r_packet, dt->socket);
        r = recvmsg(dt->socket, &msgh, 0);
        if (r >= 0)
            handle_reply(dt, &msgh);
    }
}

int    end_max_count(t_data *dt)
{
    if (dt->options_params.count > 0 && dt->end_stats.sent_nb >= dt->options_params.count)
    {
        if (dt->end_stats.recv_nb >= dt->options_params.count)
            return 1;
    }
    return 0;
}

int     end_timeout(t_data *dt)
{
    struct timeval      curr_tv; 
    
    if (dt->options_params.w_timeout > 0)
    {
        if (gettimeofday(&curr_tv, &dt->tz) != 0)
            exit_error_close(dt->socket, "ping: cannot retrieve time\n");
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