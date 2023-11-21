#include "ping_functions.h"

static char    *int_to_bin(int n, int len)
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
    ft_memset(&dt->icmp, 0, sizeof(dt->icmp));
    for (int i = 0; i < ICMP_PAYLOAD_LEN - 1; i++)
        dt->icmp.payload[i] = dt->options_params.p_payload[i];
    dt->icmp.payload[ICMP_PAYLOAD_LEN - 1] = '\0';
    dt->one_seq.icmp_seq_count++;
    // printf(C_B_RED"[DEBUG] [%s] %d"C_RES"\n", dt->icmp.payload, sizeof(dt->icmp.payload));
}

static void craft_icmp_header(t_data *dt)
{
    dt->icmp.h.type = ICMP_ECHO;
    dt->icmp.h.un.echo.id = getpid();
    dt->icmp.h.un.echo.sequence = dt->one_seq.icmp_seq_count;
    dt->icmp.h.checksum = header_checksum(&dt->icmp, sizeof(dt->icmp));
}

void craft_icmp(t_data *dt)
{
    ft_bzero(&dt->icmp, sizeof(dt->icmp));
    craft_icmp_payload(dt);
    craft_icmp_header(dt);
}