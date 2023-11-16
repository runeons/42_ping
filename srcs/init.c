#include "ft_ping.h"

void init_data(t_data *dt)
{
    dt->input_dest = "";
    dt->act_options = NULL;
    dt->resolved_address = NULL; // do I need to malloc ? if ((dt->resolved_address = (char *)mmalloc(sizeof(char) * MAX_IP_LEN + 1)) == NULL)
    dt->resolved_hostname = "";
    dt->socket = 0;
    // address not initialized ?
    dt->address.sin_family = AF_INET;
    dt->address.sin_port = 0;
    dt->address.sin_addr.s_addr = INADDR_ANY;

    // packet
    // tz
    dt->init_tv.tv_sec = 0;
    dt->init_tv.tv_usec = 0;

    // seq
    dt->one_seq.bytes = 0;
    dt->one_seq.icmp_seq = 0;
    dt->one_seq.ttl = TTL_VALUE;
    // dt->one_seq.send_tv
    // dt->one_seq.receive_tv

    dt->end_stats.sent_nb = 0;
    dt->end_stats.recv_nb = 0;
}
