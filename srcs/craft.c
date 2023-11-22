#include "ping_functions.h"

// Le complément à un sur 16 bits (= inversement de tous les bits)
// de la somme des compléments à un de l'en-tête Internet
// pris par mots de 16 bits.

static unsigned short header_checksum(void *packet, int len)
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
    // printf("checksum: %d, checksum[%s]\n", checksum, int_to_bin(checksum, 16));
	return checksum;
}

static void craft_icmp_payload(t_data *dt)
{
    ft_memset(&dt->crafted_icmp, 0, sizeof(dt->crafted_icmp));
    for (int i = 0; i < ICMP_PAYLOAD_LEN - 1; i++)
        dt->crafted_icmp.payload[i] = dt->options_params.p_payload[i];
    dt->crafted_icmp.payload[ICMP_PAYLOAD_LEN - 1] = '\0';
    dt->one_seq.icmp_seq_count++;
    // printf(C_B_RED"[DEBUG] [%s] %d"C_RES"\n", dt->crafted_icmp.payload, sizeof(dt->crafted_icmp.payload));
}

static void craft_icmp_header(t_data *dt)
{
    dt->crafted_icmp.h.type = ICMP_ECHO_REQUEST;
    dt->crafted_icmp.h.un.echo.id = getpid();
    dt->crafted_icmp.h.un.echo.sequence = dt->one_seq.icmp_seq_count;
    dt->crafted_icmp.h.checksum = header_checksum(&dt->crafted_icmp, sizeof(dt->crafted_icmp));
}

void craft_icmp(t_data *dt)
{
    ft_bzero(&dt->crafted_icmp, sizeof(dt->crafted_icmp));
    craft_icmp_payload(dt);
    craft_icmp_header(dt);
}