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

void print_as_hexa(void *data, int size)
{
    const unsigned char *ptr = (const unsigned char *)data;

    for (int i = 0; i < size; i++)
        printf("%02x ", ptr[i]);
    printf("\n");
}

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
    dt->one_seq.bytes = sizeof(*msgh);
    dt->end_stats.recv_nb++;
    save_packet(dt);
    if (dt->one_seq.final_packet.icmp->type == 0 && dt->one_seq.final_packet.icmp->code == 0)
        printf(C_G_CYAN"[DEBUG] WORKING"C_RES"\n");
    else
        printf(C_G_CYAN"[DEBUG] ERROR"C_RES"\n");
    display_ping_sequence(dt);
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
        init_recv_msg(&msgh, dt->one_seq.r_packet, dt->address);
        r = recvmsg(dt->socket, &msgh, 0);
        if (r < 0)
            warning_error(C_G_RED"packet receiving failure: %s"C_RES"\n", strerror(r));
        else
            handle_reply(dt, &msgh);
    }
}

void ping_sequence(t_data *dt)
{
    usleep(dt->options_params.seq_interval_us);
    craft_icmp(dt);
    debug_crafted_icmp(&dt->crafted_icmp);
    send_icmp_and_receive_packet(dt);
}