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

void    debug_icmp_packet(t_data *dt)
{
    if (DEBUG == 1)
    {
        printf(C_G_RED"[DEBUG] ICMP packet"C_RES"\n");
        printf("sizeof(dt->icmp): %lu\n", sizeof(dt->icmp));
        printf("dt->icmp.payload: %s\n", dt->icmp.payload);
        printf("sizeof(dt->icmp.payload): %lu\n", sizeof(dt->icmp.payload));
        printf("dt->one_seq.icmp_seq: %d\n", dt->one_seq.icmp_seq_count);
        printf("dt->icmp.h.type: %d\n", dt->icmp.h.type);
        printf("dt->icmp.h.code: %d\n", dt->icmp.h.code);
        printf("dt->icmp.h.checksum: %d\n", dt->icmp.h.checksum);
        printf("dt->icmp.h.un.echo.id: %d\n", dt->icmp.h.un.echo.id);
        printf("dt->icmp.h.un.echo.sequence: %d\n", dt->icmp.h.un.echo.sequence);
        printf("\n");
    }
}

void    debug_packet(t_icmp *p)
{
    if (DEBUG == 1)
    {
        printf(C_G_RED"[DEBUG] crafted packet"C_RES"\n");
        printf("sizeof(p): %lu\n", sizeof(p));
        printf("p->payload: %s\n", p->payload);
        printf("sizeof(p->payload): %lu\n", sizeof(p->payload));
        printf("p->h.type: %d\n", p->h.type);
        printf("p->h.code: %d\n", p->h.code);
        printf("p->h.checksum: %d\n", p->h.checksum);
        printf("p->h.un.echo.id: %d\n", p->h.un.echo.id);
        printf("p->h.un.echo.sequence: %d\n", p->h.un.echo.sequence);
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

void    debug_recv_packet(void *packet)
{
    if (DEBUG == 1)
    {
        struct iphdr *ip = packet;
    	struct icmphdr *icmp = packet + IP_HEADER_LEN;

        printf(C_G_RED"[DEBUG] recv packet"C_RES"\n");
        printf("ip->version: %x\n", ip->version);
        printf("ip->ihl: %x\n", ip->ihl);
        printf("ip->tos: %hhx\n", ip->tos);
        printf("ntohs(ip->tot_len): %hx\n", ntohs(ip->tot_len));
        printf("ip->id: %04hx\n", ip->id);
        printf("ntohs(ip->frag_off): %hx\n", ntohs(ip->frag_off));
        printf("ip->ttl: %hhx\n", ip->ttl);
        printf("ip->protocol: %hhx\n", ip->protocol);
        printf("ip->check: %04hx\n", ip->check);
        printf("ip->saddr: %08x\n", ip->saddr);
        printf("ip->daddr: %08x\n", ip->daddr);
        printf("icmp->type: %hhx\n", icmp->type);
        printf("icmp->code: %hhx\n", icmp->code);
        printf("icmp->checksum: %04hx\n", icmp->checksum);
        printf("ntohs(icmp->un.echo.id): %x\n", ntohs(icmp->un.echo.id));
        printf("ntohs(icmp->un.echo.sequence): %x\n", ntohs(icmp->un.echo.sequence));
        printf("\n");

    }
}
