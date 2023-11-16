#include "ft_ping.h"

void open_socket(t_data *dt)
{
    int ttl_value;
    int r;

    r = 0;
    ttl_value = TTL_VALUE + 1;
    dt->socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (dt->socket < 0)
    {
        printf("errno %d %s\n", errno, strerror(errno));
        exit_error("socket error: Check that you have the correct rights.\n");
    }

    struct timeval tv_out;
	tv_out.tv_sec = 1;
	tv_out.tv_usec = 0;

    // setting TTL option
    r = setsockopt(dt->socket, IPPROTO_IP, IP_TTL, &ttl_value, sizeof(ttl_value)); // IPPROTO_IP or SOL_IP or SOL_SOCKET ?
    if (r != 0)
    {
        printf("r: %d\n", r);
        dprintf(2, "setsockopt1: %s\n", strerror(-1));
        printf("errno %d %s\n", errno, strerror(errno));
        exit_error("socket error: Exiting program.\n");
    }
    // setting timeout option
    r = setsockopt(dt->socket, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out));
    if (r != 0)
    {
        dprintf(2, "setsockopt2: %s\n", strerror(-1));
        printf("errno %d %s\n", errno, strerror(errno));
        exit_error("socket error: Exiting program.\n");
    }
}