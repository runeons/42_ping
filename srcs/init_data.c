#include "ping_functions.h"

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

void    init_recv_buf(struct msghdr *msg)
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

