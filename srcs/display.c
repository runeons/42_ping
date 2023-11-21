#include "ping_functions.h"

void    display_ping_init(t_data *dt)
{
    printf("PING %s (%s) %lu(%d) bytes of data.\n", dt->input_dest, dt->resolved_address, sizeof(dt->icmp.payload), ICMP_TOTAL_LEN); // sizeof(dt->icmp)
}

void    display_ping_sequence(t_data *dt)
{
    int time;

    time = (dt->one_seq.receive_tv.tv_sec - dt->one_seq.send_tv.tv_sec) * 1000000 + dt->one_seq.receive_tv.tv_usec - dt->one_seq.send_tv.tv_usec;
    printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n", dt->one_seq.bytes, dt->resolved_address, dt->one_seq.icmp_seq_count, dt->one_seq.ttl, (float)time / 1000);
}

void    display_ping_end_stats(t_data *dt)
{
    int             ratio;
    int             time;
    struct timeval  final_tv;

    if (gettimeofday(&final_tv, &dt->tz) != 0)
            exit_error("time error: Cannot retrieve time\n");
    ratio = 100 - ((dt->end_stats.recv_nb / dt->end_stats.sent_nb) * 100);
    time = (final_tv.tv_sec - dt->init_tv.tv_sec) * 1000000 + final_tv.tv_usec - dt->init_tv.tv_usec;
    printf("--- %s ping statistics ---\n", dt->input_dest);
    printf("%d packets transmitted, %d received, %d%% packet loss, time %d ms\n", dt->end_stats.sent_nb, dt->end_stats.recv_nb, ratio, time / 1000);
}
