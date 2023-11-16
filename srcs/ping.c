#include "ping_functions.h"

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

void    _init_recv_buf(struct msghdr *msg)
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

void receive_failure(t_data *dt, struct msghdr buf, int recv_ret)
{
    // wait or timeout
    dprintf(2, "packet receiving failure: %s\n", strerror(recv_ret));
    // debug_buf(buf);  
}

void receive_success(t_data *dt, struct msghdr buf)
{
    if (gettimeofday(&dt->one_seq.receive_tv, &dt->tz) != 0)
        exit_error("time error: Cannot retrieve time\n");
    dt->one_seq.bytes = sizeof(buf);
    dt->end_stats.recv_nb++;
    display_ping_sequence(dt);      
    // debug_buf(buf);  
}

void receive_packet(t_data *dt)
{
    struct msghdr buf;
    int r;

    dt->end_stats.sent_nb++;
    _init_recv_buf(&buf);
    r = recvmsg(dt->socket, &buf, 0);
    if (r < 0)
        receive_failure(dt, buf, r);
    else
        receive_success(dt, buf);
}

void craft_icmp_payload(t_data *dt)
{
    ft_memset(&dt->packet, 0, sizeof(dt->packet));
    for (int i = 0; i < ICMP_PAYLOAD_LEN - 1; i++)
        dt->packet.payload[i] = 'A';
    dt->packet.payload[ICMP_PAYLOAD_LEN - 1] = '\0';
    dt->one_seq.icmp_seq++;
}

void craft_icmp_packet(t_data *dt)
{
    dt->packet.h.type = ICMP_ECHO;
    dt->packet.h.un.echo.id = getpid();
    dt->packet.h.un.echo.sequence = dt->one_seq.icmp_seq;
    dt->packet.h.checksum = checksum(&dt->packet, sizeof(dt->packet));
}

void    send_and_receive_packet(t_data *dt)
{
    int r = 0;

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

void ping_sequence(t_data *dt)
{
    usleep(SEQUENCE_TIME);
    craft_icmp_payload(dt);
    craft_icmp_packet(dt);
    // debug_icmp_packet(dt);
    send_and_receive_packet(dt);
}