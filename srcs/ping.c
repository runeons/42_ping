#include "ping_functions.h"

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
    dt->one_seq.icmp_seq++;
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


void    _init_buf(struct msghdr *msg)
{
    struct icmphdr  *icmp_control;
    struct iovec    *iov;
    char            *buffer;

    if (!(buffer = mmalloc(sizeof(char) * 1024)))
        exit_error("Malloc error (buffer)\n");
    ft_bzero(buffer, 1024);
    if (!(iov = (struct iovec *)mmalloc(sizeof(struct iovec))))
        exit_error("Malloc error (iov)\n");
    ft_bzero(iov, sizeof(*iov));
    if (!(icmp_control = (struct icmphdr *)mmalloc(sizeof(struct icmphdr))))
        exit_error("Malloc error (icmp_control)\n");
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


void receive_packet(t_data *dt)
{
    struct msghdr buf;
    int r;

    dt->end_stats.sent_nb++;
    _init_buf(&buf);
    r = recvmsg(dt->socket, &buf, 0);
    // printf("receive_packet %d\n", r);
    if (r < 0)
    {
        // wait or timeout
        dprintf(2, "packet receiving failure: %s\n", strerror(r));
        // debug_buf(buf);  

    }
    else
    {
        // printf("r: %d %d\n", r, dt->socket);
        if (gettimeofday(&dt->one_seq.receive_tv, &dt->tz) != 0)
            exit_error("time error: Cannot retrieve time\n");
        dt->one_seq.bytes = sizeof(buf);
        dt->end_stats.recv_nb++;
        display_ping_sequence(dt);      
        // printf("r: %d\n", r);
        // debug_buf(buf);  
    }
}

void ping_sequence(t_data *dt)
{
    int r;

    r = 0;
    usleep(SLEEP_WAIT);
    craft_icmp_payload(dt);
    dt->packet.h.type = ICMP_ECHO;
    dt->packet.h.un.echo.id = getpid();
    dt->packet.h.un.echo.sequence = dt->one_seq.icmp_seq;
    dt->packet.h.checksum = checksum(&dt->packet, sizeof(dt->packet));
    // debug_icmp_packet(dt);
    if (gettimeofday(&dt->one_seq.send_tv, &dt->tz) != 0)
        exit_error("time error: Cannot retrieve time\n");
    r = sendto(dt->socket, &dt->packet, sizeof(dt->packet), 0, (struct sockaddr*)&dt->address, sizeof(dt->address));
    if (r <= 0)
        dprintf(2, "packet sending failure: %s\n", strerror(r));
    else if (r != sizeof(dt->packet))
        dprintf(2, "packet not entirely sent: %s\n", strerror(r));
    else
        receive_packet(dt);
}