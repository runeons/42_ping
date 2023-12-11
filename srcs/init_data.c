#include "ping_functions.h"

void init_data(t_data *dt, t_parsed_cmd *parsed_cmd)
{
    dt->input_dest = "";
    dt->act_options = parsed_cmd->act_options;
    dt->resolved_address = NULL;
    dt->resolved_hostname = "";
    dt->socket = 0;
    memset(&(dt->address), 0, sizeof(dt->address)); // ADDED
    dt->address.sin_family = AF_INET;
    dt->address.sin_port = 0;
    dt->address.sin_addr.s_addr = INADDR_ANY;

    // packet
    dt->init_tv.tv_sec = 0;
    dt->init_tv.tv_usec = 0;

    // seq
    dt->one_seq.bytes = 0;
    dt->one_seq.icmp_seq_count = -1;
    dt->one_seq.ttl = 0;
    dt->one_seq.time = 0;

    dt->end_stats.sent_nb = 0;
    dt->end_stats.recv_nb = 0;
    dt->end_stats.times = NULL;
}

void    init_recv_msgh(struct msghdr *msg, char *r_packet)
{
    struct iovec        *iov;
    char                *msg_control;
    struct sockaddr_in  *dst;

    if (!(iov = mmalloc(sizeof(struct iovec))))
        exit_error("ping: malloc failure.\n");
    if (!(msg_control = mmalloc(sizeof(char) * STR_BUFFER_LEN)))
        exit_error("ping: malloc failure.\n");
    if (!(dst = mmalloc(sizeof(struct sockaddr_in))))
        exit_error("ping: malloc failure.\n");
    ft_bzero(r_packet, IP_TOTAL_LEN);
    iov->iov_base = r_packet;
    iov->iov_len = IP_TOTAL_LEN;
	msg->msg_name = dst;
	msg->msg_namelen = sizeof(*dst);
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;
	msg->msg_control = msg_control;
	msg->msg_controllen = sizeof(msg_control);
	msg->msg_flags = 0;
}
