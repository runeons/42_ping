#include "ping_functions.h"

void    debug_addrinfo(struct addrinfo *ai)
{
    if (DEBUG == 1)
    {
        printf(C_G_RED"[DEBUG]"C_RES" addrinfo\n");
        printf("ai_family: %d\n", ai->ai_family);
        printf("ai_socktype: %d\n", ai->ai_socktype);
        printf("ai_addr: %s\n", inet_ntoa(((struct sockaddr_in *)ai->ai_addr)->sin_addr));
        printf("\n");
    }
}

void    debug_crafted_icmp(t_icmp *crafted_icmp)
{
    if (DEBUG == 1)
    {
        printf(C_G_RED"[DEBUG] crafted t_icmp"C_RES"\n");
        printf("sizeof(crafted_icmp): %lu\n", sizeof(crafted_icmp));
        printf("sizeof(crafted_icmp->payload): %lu\n", sizeof(crafted_icmp->payload));
        printf("payload: %s\n", crafted_icmp->payload);
        printf("h.type: %d\n", crafted_icmp->h.type);
        printf("h.code: %d\n", crafted_icmp->h.code);
        printf("h.checksum: %d\n", crafted_icmp->h.checksum);
        printf("h.un.echo.id: %d\n", crafted_icmp->h.un.echo.id);
        printf("h.un.echo.sequence: %d\n", crafted_icmp->h.un.echo.sequence);
        printf("h.un.echo.id: %x\n", crafted_icmp->h.un.echo.id);
        printf("h.un.echo.sequence: %x\n", crafted_icmp->h.un.echo.sequence);
        printf("\n");
    }
}

void    debug_msghdr(struct msghdr msg)
{
    if (DEBUG == 1)
    {
        printf(C_G_RED"[DEBUG] recv buf"C_RES"\n");
        printf("iov_base ====> %s \n", (char *)msg.msg_iov[0].iov_base);
        printf("iov_len ====> %lu \n", msg.msg_iov[0].iov_len);
        printf("msg_name ====> %s \n", (char *)msg.msg_name);
        printf("msg_namelen ====> %u \n", msg.msg_namelen);
        printf("msg_iovlen ====> %lu \n", msg.msg_iovlen);
        printf("msg_control ====> %s \n", (char *)msg.msg_control);
        printf("msg_controllen ====> %lu \n", msg.msg_controllen);
        printf("msg_flags ====> %d \n\n", msg.msg_flags);  
        printf("\n");
        // printf("iov_base ====> %p \n", (char *)msg.msg_iov[0].iov_base);
        // printf("iov_len ====> %x \n", msg.msg_iov[0].iov_len);
        // printf("msg_name ====> %p \n", (char *)msg.msg_name);
        // printf("msg_namelen ====> %x \n", msg.msg_namelen);
        // printf("msg_iovlen ====> %x \n", msg.msg_iovlen);
        // printf("msg_control ====> %p \n", (char *)msg.msg_control);
        // printf("msg_controllen ====> %x \n", msg.msg_controllen);
        // printf("msg_flags ====> %x \n\n", msg.msg_flags);  
        // printf("\n");

    }
}

void    debug_packet(t_packet *packet)
{
    if (DEBUG == 1)
    {
        printf(C_G_RED"[DEBUG] final_packet t_packet"C_RES"\n");
        printf("ip->version: %x\n", packet->ip->version);
        printf("ip->ihl: %x\n", packet->ip->ihl);
        printf("ip->tos: %hhx\n", packet->ip->tos);
        printf("ntohs(ip->tot_len): %hx\n", ntohs(packet->ip->tot_len));
        printf("ip->id: %04hx\n", packet->ip->id);
        printf("ntohs(packet->ip->frag_off): %hx\n", ntohs(packet->ip->frag_off));
        printf("ip->ttl: %hhx\n", packet->ip->ttl);
        printf("ip->protocol: %hhx\n", packet->ip->protocol);
        printf("ip->check: %04hx\n", packet->ip->check);
        printf("ip->saddr: %08x\n", packet->ip->saddr);
        printf("ip->daddr: %08x\n", packet->ip->daddr);
        printf("icmp->type: %hhx\n", packet->icmp->type);
        printf("icmp->code: %hhx\n", packet->icmp->code);
        printf("icmp->checksum: %04hx\n", packet->icmp->checksum);
        printf("ntohs(packet->icmp->un.echo.id): %x\n", ntohs(packet->icmp->un.echo.id));
        printf("ntohs(packet->icmp->un.echo.sequence): %x\n", ntohs(packet->icmp->un.echo.sequence));
        printf("\n");

    }
}
