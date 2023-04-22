#include "ft_ping.h"

void open_socket(t_data *dt)
{
    int ttl_value;
    int r;

    r = 0;
    ttl_value = TTL_VALUE;
    dt->socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (dt->socket < 0)
    {
        exit_error("socket error: Check that you have the correct rights.");
    }

    struct timeval tv_out;
	tv_out.tv_sec = 1;
	tv_out.tv_usec = 0;

    r = setsockopt(dt->socket, IPPROTO_IP, IP_TTL, &ttl_value, sizeof(ttl_value)); // IPPROTO_IP or SOL_IP orSOL_SOCKET ?
    if (r != 0)
    {
        dprintf(2, "setsockopt1: %s\n", strerror(-1));
        exit_error("socket error: Exiting program.");
    }
    r = setsockopt(dt->socket, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));
    if (r != 0)
    {
        dprintf(2, "setsockopt2: %s\n", strerror(-1));
        exit_error("socket error: Exiting program.");
    }

}