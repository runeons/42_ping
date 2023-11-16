#include "ping_functions.h"

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
