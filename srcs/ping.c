#include "ft_ping.h"

void print_icmp_packet(t_data *dt)
{
    printf("sizeof(dt->packet): %lu\n", sizeof(dt->packet));
    printf("dt->packet.payload: %s\n", dt->packet.payload);
    printf("sizeof(dt->packet.payload): %lu\n", sizeof(dt->packet.payload));
    printf("dt->sequence: %d\n", dt->sequence);
    printf("dt->packet.h.type: %d\n", dt->packet.h.type);
    printf("dt->packet.h.code: %d\n", dt->packet.h.code);
    printf("dt->packet.h.checksum: %d\n", dt->packet.h.checksum);
    printf("dt->packet.h.un.echo.id: %d\n", dt->packet.h.un.echo.id);
    printf("dt->packet.h.un.echo.sequence: %d\n", dt->packet.h.un.echo.sequence);
}

void craft_icmp_payload(t_data *dt)
{
    int i;

    i = 0;
    ft_bzero(&dt->packet, sizeof(dt->packet));
    while (i < ICMP_PAYLOAD_LEN)
    {
		dt->packet.payload[i] = 'A';
        i++;
    }
    dt->packet.payload[ICMP_PAYLOAD_LEN - 1] = '\0';
    dt->sequence++;
}

// Calculating the Check Sum
unsigned short checksum(void *b, int len)
{ unsigned short *buf = b;
	unsigned int sum=0;
	unsigned short result;

	for ( sum = 0; len > 1; len -= 2 )
		sum += *buf++;
	if ( len == 1 )
		sum += *(unsigned char*)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

void    print_init_ping(t_data *dt)
{
    printf("PING %s (%s) %lu(%lu) bytes of data.\n", dt->param, dt->ip, sizeof(dt->packet.payload), sizeof(dt->packet));
}

void    print_ping(t_data *dt)
{
    int time;

    time = (dt->receive_tv.tv_sec - dt->send_tv.tv_sec) * 1000000 + dt->receive_tv.tv_usec - dt->send_tv.tv_usec;
    printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n", dt->bytes, dt->ip, dt->sequence, dt->ttl, (float)time / 1000);
}

void    print_statistics(t_data *dt)
{
    int ratio;
    int time;

    ratio = 100 - ((dt->recv_nb / dt->sent_nb) * 100);
    time = (dt->receive_tv.tv_sec - dt->init_tv.tv_sec) * 1000000 + dt->receive_tv.tv_usec - dt->init_tv.tv_usec;
    printf("--- %s ping statistics ---\n", dt->param);
    printf("%d packets transmitted, %d received, %d%% packet loss, time %d ms\n", dt->sent_nb, dt->recv_nb, ratio, time / 1000);
}

void    init_buf(struct msghdr *msg)
{
    struct icmphdr icmp_control;
    struct iovec iov[1];
    char buffer[1024];  // Buffer de messages de taille 1024
    // char control[32];  // Données de contrôle de taille sizeof(struct timeval)

    // build control structure
    memset(&icmp_control, 0, sizeof(struct icmphdr));
    icmp_control.type = 4;

    iov[0].iov_base = buffer;
    iov[0].iov_len = sizeof(buffer);
    msg->msg_name = NULL;
    msg->msg_namelen = 0;
    msg->msg_iov = iov;
    msg->msg_iovlen = 1;
    msg->msg_control = &icmp_control;
    msg->msg_controllen = sizeof(icmp_control);
    msg->msg_flags = 0;
}

void    print_buf(struct msghdr msg)
{
    // printf("msg_iov ====> %lu \n", msg.msg_iov);
    printf("iov_base ====> %s \n", (char *)msg.msg_iov[0].iov_base);
    printf("iov_len ====> %lu \n", msg.msg_iov[0].iov_len);
    printf("msg_name ====> %s \n", (char *)msg.msg_name);
    printf("msg_namelen ====> %uu \n", msg.msg_namelen);
    printf("msg_iovlen ====> %lu \n", msg.msg_iovlen);
    printf("msg_control ====> %s \n", (char *)msg.msg_control);
    printf("msg_controllen ====> %lu \n", msg.msg_controllen);
    printf("msg_flags ====> %d \n", msg.msg_flags);
}

void receive_packet(t_data *dt)
{
    struct msghdr buf;
    int r;

    dt->sent_nb++;
    init_buf(&buf);
    r = recvmsg(dt->socket, &buf, 0);
    if (r < 0)
        dprintf(2, "packet receiving failure: %s\n", strerror(r));
    else
    {
        if (gettimeofday(&dt->receive_tv, &dt->tz) != 0)
            exit_error("time error: Cannot retrieve time");
        dt->bytes = sizeof(buf);
        dt->recv_nb++;
        print_ping(dt);      
        printf("r: %d\n", r);
        print_buf(buf);  
        // printf("ICMP_ECHOREPLY: %d\n", ICMP_ECHOREPLY);
        // if (buf.msg_type == ICMP_ECHOREPLY)
        //     printf("ICMP type: %d, code: %d\n", icmp->type, icmp->code);
        // else
        //     exit_error("Not ICMP_ECHOREPLY\n");
    }
}

void ping(t_data *dt)
{
    int r;

    r = 0;
    usleep(SLEEP_WAIT);
    craft_icmp_payload(dt);
    dt->packet.h.type = ICMP_ECHO;
    dt->packet.h.un.echo.id = getpid();
    dt->packet.h.un.echo.sequence = dt->sequence;
    dt->packet.h.checksum = checksum(&dt->packet, sizeof(dt->packet));
    // print_icmp_packet(dt);
    if (gettimeofday(&dt->send_tv, &dt->tz) != 0)
        exit_error("time error: Cannot retrieve time");
    if ((r = sendto(dt->socket, &dt->packet, sizeof(dt->packet), 0, (struct sockaddr*)&dt->address, sizeof(dt->address))) <= 0)
        dprintf(2, "packet sending failure: %s\n", strerror(r));
    else
        receive_packet(dt);
}