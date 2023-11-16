#include "ping_functions.h"

char    *_int_to_bin(int n, int len)
{
    char    *bin;
    int     k = 0;

    bin = (char*)mmalloc(sizeof(char) * len);
    if (bin == NULL)
        exit_error("Malloc error.");
    for (unsigned i = (1 << (len - 1)); i > 0; i = i / 2)
        bin[k++] = (n & i) ? '1' : '0';
    bin[k] = '\0';
    return bin;
}

// Le complément à un sur 16 bits (= inversement de tous les bits)
// de la somme des compléments à un de l'en-tête Internet
// pris par mots de 16 bits.

unsigned short _header_checksum(void *packet, int len)
{
    unsigned short  *tmp;
	unsigned int    checksum;

    tmp = packet;
    // printf("tmp: %d, len: %d\n", *tmp, len);
    checksum = 0;
    while (len > 1)
    {
        checksum += *tmp++;
        len -= sizeof(*tmp);
    }
	if (len == 1) // should not go in there because 16 = even 
		checksum += *(unsigned char *)tmp;
	checksum = (unsigned short)(~((checksum >> 16) + (checksum & 0xFFFF))); // complément à un = inversement de tous les bits
    // printf("checksum: %d, checksum[%s]\n", checksum, _int_to_bin(checksum, 16));
	return checksum;
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
    init_recv_buf(&buf);
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
    dt->packet.h.checksum = _header_checksum(&dt->packet, sizeof(dt->packet));
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