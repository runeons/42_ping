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
    dt->one_seq.final_packet.ip = (void *)dt->one_seq.r_packet;
    dt->one_seq.final_packet.icmp = (void *)dt->one_seq.r_packet + IP_HEADER_LEN;
    dt->one_seq.final_packet.payload = (void *)dt->one_seq.r_packet + IP_HEADER_LEN + ICMP_HEADER_LEN;
}

static void    handle_reply(t_data *dt, struct msghdr *msgh)
{
    if (gettimeofday(&dt->one_seq.receive_tv, &dt->tz) != 0)
        exit_error("time error: Cannot retrieve time\n");
    save_packet(dt);
    if (dt->one_seq.final_packet.icmp->type == ICMP_ECHO_REPLY)
    {
        dt->one_seq.bytes = sizeof(*msgh);
        display_ping_OK(dt);
        dt->end_stats.recv_nb++;
    }
    else
    {
        dt->one_seq.bytes = 0;
        dt->end_stats.errors_nb++;
        if (dt->one_seq.final_packet.icmp->type == ICMP_ERR_UNREACHABLE)
            display_ping_unreachable(dt);
        else
            warning_error(C_G_BLUE"UNHANDLED type %d"C_RES"\n", dt->one_seq.final_packet.icmp->type);
    }
    debug_packet(&(dt->one_seq.final_packet));
}

static void    send_icmp_and_receive_packet(t_data *dt)    
{
    int         r = 0;	
    struct      msghdr msgh;

    if (gettimeofday(&dt->one_seq.send_tv, &dt->tz) != 0)
        exit_error("time error: Cannot retrieve time\n");
    r = sendto(dt->socket, &dt->crafted_icmp, sizeof(dt->crafted_icmp), 0, (struct sockaddr*)&dt->address, sizeof(dt->address));
    if (r <= 0)
        warning_error(C_G_RED"packet sending failure: %s"C_RES"\n", strerror(r));
    else if (r != sizeof(dt->crafted_icmp))
        warning_error(C_G_RED"packet not entirely sent: %s"C_RES"\n", strerror(r));
    else
    {
        dt->end_stats.sent_nb++;
        init_recv_msgh(&msgh, dt->one_seq.r_packet, dt->address);
        r = recvmsg(dt->socket, &msgh, 0);
        if (r >= 0)
            handle_reply(dt, &msgh);
        // else
            // warning_error(C_G_RED"packet receiving failure: %s"C_RES"\n", strerror(r));
    }
}

void ping_sequence(t_data *dt)
{
    if (dt->options_params.count > 0 && dt->end_stats.sent_nb >= dt->options_params.count)
    {
        g_ping = 0;
        return ;
    }
    usleep(dt->options_params.seq_interval_us);
    craft_icmp(dt);
    debug_crafted_icmp(&dt->crafted_icmp);
    send_icmp_and_receive_packet(dt);
}