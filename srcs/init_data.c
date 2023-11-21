#include "ping_functions.h"

void init_data(t_data *dt, t_parsed_cmd *parsed_cmd)
{
    dt->input_dest = "";
    dt->act_options = parsed_cmd->act_options;
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
    dt->one_seq.icmp_seq_count = 0;
    dt->one_seq.ttl = TTL_VALUE;
    // dt->one_seq.send_tv
    // dt->one_seq.receive_tv

    dt->end_stats.sent_nb = 0;
    dt->end_stats.recv_nb = 0;
    dt->end_stats.errors_nb = 0;
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
    icmp_control->type = 0;
    iov->iov_base = buffer;
    iov->iov_len = sizeof(buffer);
    msg->msg_name = NULL;
    msg->msg_namelen = 0;
    msg->msg_iov = iov;
    msg->msg_iovlen = 0;
    msg->msg_control = icmp_control;
    msg->msg_controllen = sizeof(*icmp_control);
    msg->msg_flags = 0;
}

void    init_recv_msgh(struct msghdr *msg, char *r_packet, struct sockaddr_in sockaddr)
{
	char			buffer[512];
    
    ft_bzero(r_packet, IP_TOTAL_LEN); 
	struct iovec	iov =
	{
		.iov_base = r_packet,
		.iov_len = IP_TOTAL_LEN
	};
	msg->msg_name = &sockaddr;
	msg->msg_namelen = sizeof(sockaddr);
	msg->msg_iov = &iov;
	msg->msg_iovlen = 1;
	msg->msg_control = buffer;
	msg->msg_controllen = sizeof(buffer);
	msg->msg_flags = 0;

}
