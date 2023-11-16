#include "ft_ping.h"

void    debug_addrinfo(struct addrinfo *ai)
{
    if (DEBUG == 1)
    {
        printf(C_B_RED"[DEBUG]"C_RES" addrinfo\n");
        printf("ai_family: %d\n", ai->ai_family);
        printf("ai_socktype: %d\n", ai->ai_socktype);
        printf("ai_addr: %s\n", inet_ntoa(((struct sockaddr_in *)ai->ai_addr)->sin_addr));
    }
}

void    debug_icmp_packet(t_data *dt)
{
    if (DEBUG == 1)
    {
        printf("sizeof(dt->packet): %lu\n", sizeof(dt->packet));
        printf("dt->packet.payload: %s\n", dt->packet.payload);
        printf("sizeof(dt->packet.payload): %lu\n", sizeof(dt->packet.payload));
        printf("dt->one_seq.icmp_seq: %d\n", dt->one_seq.icmp_seq);
        printf("dt->packet.h.type: %d\n", dt->packet.h.type);
        printf("dt->packet.h.code: %d\n", dt->packet.h.code);
        printf("dt->packet.h.checksum: %d\n", dt->packet.h.checksum);
        printf("dt->packet.h.un.echo.id: %d\n", dt->packet.h.un.echo.id);
        printf("dt->packet.h.un.echo.sequence: %d\n", dt->packet.h.un.echo.sequence);
    }
}

void    debug_buf(struct msghdr msg)
{
    if (DEBUG == 1)
    {
        printf("iov_base ====> %s \n", (char *)msg.msg_iov[0].iov_base);
        printf("iov_len ====> %lu \n", msg.msg_iov[0].iov_len);
        printf("msg_name ====> %s \n", (char *)msg.msg_name);
        printf("msg_namelen ====> %u \n", msg.msg_namelen);
        printf("msg_iovlen ====> %lu \n", msg.msg_iovlen);
        printf("msg_control ====> %s \n", (char *)msg.msg_control);
        printf("msg_controllen ====> %lu \n", msg.msg_controllen);
        printf("msg_flags ====> %d \n\n", msg.msg_flags);  
    }
}

void    display_ping_init(t_data *dt)
{
    printf("PING %s (%s) %lu(%d) bytes of data.\n", dt->input_dest, dt->resolved_address, sizeof(dt->packet.payload), ICMP_PACKET_LEN); // sizeof(dt->packet)
}

void    display_ping_sequence(t_data *dt)
{
    int time;

    time = (dt->one_seq.receive_tv.tv_sec - dt->one_seq.send_tv.tv_sec) * 1000000 + dt->one_seq.receive_tv.tv_usec - dt->one_seq.send_tv.tv_usec;
    printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n", dt->one_seq.bytes, dt->resolved_address, dt->one_seq.icmp_seq, dt->one_seq.ttl, (float)time / 1000);
}

void    display_ping_end_stats(t_data *dt)
{
    int             ratio;
    int             time;
    struct timeval  final_tv;

    if (gettimeofday(&final_tv, &dt->tz) != 0)
            exit_error("time error: Cannot retrieve time\n");
    ratio = 100 - ((dt->end_stats.recv_nb / dt->end_stats.sent_nb) * 100);
    time = (final_tv.tv_sec - dt->init_tv.tv_sec) * 1000000 + final_tv.tv_usec - dt->init_tv.tv_usec;
    printf("--- %s ping statistics ---\n", dt->input_dest);
    printf("%d packets transmitted, %d received, %d%% packet loss, time %d ms\n", dt->end_stats.sent_nb, dt->end_stats.recv_nb, ratio, time / 1000);
}
