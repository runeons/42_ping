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

void receive_packet(t_data *dt)
{
    struct msghdr buf;
    int r;

    r = recvmsg(dt->socket, &buf, 0);
    if (r < 0)
        dprintf(2, "packet receiving failure: %s\n", strerror(r));
    else
    {
        printf("Packet received\n");
        printf("buf.msg_name: %s\n", (char *)buf.msg_name);
        printf("buf.msg_flags: %d\n", (buf.msg_flags));
        printf("buf.msg_iovlen: %zu\n", buf.msg_iovlen);
    }
}

void ping(t_data *dt)
{
    int r;

    r = 0;
    craft_icmp_payload(dt);
    dt->packet.h.type = ICMP_ECHO;
    dt->packet.h.un.echo.id = getpid();
    dt->packet.h.un.echo.sequence = dt->sequence;
    dt->packet.h.checksum = checksum(&dt->packet, sizeof(dt->packet));
    // print_icmp_packet(dt);
    if ((r = sendto(dt->socket, &dt->packet, sizeof(dt->packet), 0, (struct sockaddr*)&dt->address, sizeof(dt->address))) <= 0)
        dprintf(2, "packet sending failure: %s\n", strerror(r));
    else
        receive_packet(dt);
}