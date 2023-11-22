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
        printf(C_G_RED"[DEBUG] RECEIVED PACKET"C_RES"\n");
        printf(C_G_RED"[DEBUG] IP"C_RES"\n");
        printf("    version: %x\n", packet->ip->version);
        printf("    ihl: %x\n", packet->ip->ihl);
        printf("    tos: %d\n", packet->ip->tos);
        printf("    tot_len: %hx\n", packet->ip->tot_len);
        printf("    id: %04x\n", packet->ip->id);
        printf("    frag_off: %hx\n", ntohs(packet->ip->frag_off)); // TO CHECK
        printf("    ttl: %hhx\n", packet->ip->ttl);
        printf("    protocol: %hhx\n", packet->ip->protocol);
        printf("    check: 0x%04x\n", packet->ip->check);
        printf("    saddr: %08x\n", packet->ip->saddr);
        printf("    daddr: %08x\n", packet->ip->daddr);
        printf(C_G_RED"[DEBUG] ICMP"C_RES"\n");
        printf("    type: %d\n", packet->icmp->type);
        printf("    code: %d\n", packet->icmp->code);
        printf("    checksum: 0x%04x\n", packet->icmp->checksum);
        printf("    id: 0x%04x\n", packet->icmp->un.echo.id);
        printf("    sequence: 0x%04x\n", packet->icmp->un.echo.sequence);
        printf(C_G_RED"[DEBUG] ICMP IN PAYLOAD"C_RES"\n");
        struct icmphdr  *icmp_in_payload = (struct icmphdr *)((char *)packet->icmp + IP_HEADER_LEN + ICMP_HEADER_LEN);
        printf("    id: 0x%04x\n", icmp_in_payload->un.echo.id);
        printf("    sequence: 0x%04x\n", icmp_in_payload->un.echo.sequence);
        printf("\n");

    }
}

void debug_time(void *content)
{
    if (content)
    {
        int *t = (int *)content;
        printf("time: %d, \n", *t);
    }
}