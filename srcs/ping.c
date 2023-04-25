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

// char* toBinary(int n, int len)
// {
//     char* binary = (char*)malloc(sizeof(char) * len);
//     int k = 0;
//     for (unsigned i = (1 << (len - 1)); i > 0; i = i / 2) {
//         binary[k++] = (n & i) ? '1' : '0';
//     }
//     binary[k] = '\0';
//     return binary;
// }

// unsigned short checksum(void *b, int len)
// {
//     unsigned short *buf = b;
//     unsigned int sum = 0;
//     unsigned short result;

//     for (sum = 0; len > 1; len -= 2)
//         sum += *buf++;
//     if (len == 1)
//         sum += *(unsigned char *)buf;
//     sum = (sum >> 16) + (sum & 0xFFFF);
//     sum += (sum >> 16);
//     result = ~sum;

//     return result;
// }

unsigned short checksum(void *packet, int len)
{
    unsigned short  *tmp;
	unsigned int    checksum;

    tmp = packet;
    // printf("tmp: %d, len: %d\n", *tmp, len);
    checksum = 0;
    while (len > 1)
    {
        checksum += *tmp++;
        // printf("len: %d, tmp: %hu, checksum: %d, tmp[%s], checksum[%s]\n", len, *tmp, checksum, toBinary(*tmp, 16), toBinary(checksum, 16));
        len -= sizeof(unsigned short);
    }
	if (len == 1)
		checksum += *(unsigned char*)tmp;
    // printf("sum1: %d, checksum[%s]\n", checksum, toBinary(checksum, 16));
	checksum = (checksum >> 16) + (checksum & 0xFFFF);
    // printf("sum2: %d, checksum[%s]\n", checksum, toBinary(checksum, 16));
    // printf("checksum >> 16 [%s]\n", toBinary(checksum >> 16, 16));
	checksum += (checksum >> 16);
    // printf("sum3: %d, checksum[%s]\n", checksum, toBinary(checksum, 32));
	checksum = (unsigned short)(~checksum);
    // printf("checksum: %d, checksum[%s]\n", checksum, toBinary(checksum, 16));
	return checksum;
}


void    print_init_ping(t_data *dt)
{
    printf("PING %s (%s) %lu(%d) bytes of data.\n", dt->param, dt->ip, sizeof(dt->packet.payload), ICMP_PACKET_LEN); // sizeof(dt->packet)
}

void    print_ping(t_data *dt)
{
    int time;

    time = (dt->receive_tv.tv_sec - dt->send_tv.tv_sec) * 1000000 + dt->receive_tv.tv_usec - dt->send_tv.tv_usec;
    printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n", dt->bytes, dt->ip, dt->sequence, dt->ttl, (float)time / 1000);
}

void    print_statistics(t_data *dt)
{
    int             ratio;
    int             time;
    struct timeval  final_tv;

    if (gettimeofday(&final_tv, &dt->tz) != 0)
            exit_error("time error: Cannot retrieve time");
    ratio = 100 - ((dt->recv_nb / dt->sent_nb) * 100);
    time = (final_tv.tv_sec - dt->init_tv.tv_sec) * 1000000 + final_tv.tv_usec - dt->init_tv.tv_usec;
    printf("--- %s ping statistics ---\n", dt->param);
    printf("%d packets transmitted, %d received, %d%% packet loss, time %d ms\n", dt->sent_nb, dt->recv_nb, ratio, time / 1000);
}

void    init_buf(struct msghdr *msg)
{
    struct icmphdr  *icmp_control;
    struct iovec    *iov;
    char            *buffer;

    if (!(buffer = mmalloc(sizeof(char) * 1024)))
        exit_error("Malloc error (buffer)");
    ft_bzero(buffer, 1024);
    if (!(iov = (struct iovec *)mmalloc(sizeof(struct iovec))))
        exit_error("Malloc error (iov)");
    ft_bzero(iov, sizeof(*iov));
    if (!(icmp_control = (struct icmphdr *)mmalloc(sizeof(struct icmphdr))))
        exit_error("Malloc error (icmp_control)");
    ft_bzero(icmp_control, sizeof(*icmp_control));
    icmp_control->type = 4;
    iov->iov_base = buffer;
    iov->iov_len = sizeof(buffer);
    msg->msg_name = NULL;
    msg->msg_namelen = 0;
    msg->msg_iov = iov;
    msg->msg_iovlen = 1;
    msg->msg_control = icmp_control;
    msg->msg_controllen = sizeof(*icmp_control);
    msg->msg_flags = 4;
}

void    print_buf(struct msghdr msg)
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

void    print_verbose(t_data *dt)
{
    if (dt->flag_v == 1)
    {
        dprintf(2, "%s\n", dt->v_buf);
    }
}

void receive_packet(t_data *dt)
{
    struct msghdr buf;
    int r;

    dt->sent_nb++;
    init_buf(&buf);
    r = recvmsg(dt->socket, &buf, 0);
    if (r < 0)
    {
        dprintf(2, "packet receiving failure: %s\n", strerror(r));
        // sprintf(dt->v_buf, "packet receiving failure: %s\n", strerror(r));
        print_verbose(dt);
    }
    else
    {
        if (gettimeofday(&dt->receive_tv, &dt->tz) != 0)
            exit_error("time error: Cannot retrieve time");
        dt->bytes = sizeof(buf);
        dt->recv_nb++;
        print_ping(dt);      
        // printf("r: %d\n", r);
        // print_buf(buf);  
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