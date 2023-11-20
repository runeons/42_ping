#include "ping_functions.h"

void   option_p(t_data *dt)
{
    if (is_activated_option(dt->act_options, 'p'))
    {
        strncpy(dt->options_params.p_payload, get_option(dt->act_options, 'p')->param, ICMP_PAYLOAD_LEN);
        dt->options_params.p_payload[ICMP_PAYLOAD_LEN - 1] = '\0';
    }
    else
    {
        for (int i = 0; i < ICMP_PAYLOAD_LEN - 1; i++)
            dt->options_params.p_payload[i] = 'A';
    }
}

int ft_isstrfloat(const char *s)
{
    int i           = 0;
    int nb_dot      = 0;
    int nb_digit    = 0;

    if (s && s[i] && (s[i] == '-' || s[i] == '+') && s[i + 1])
        i++;
    while (s && s[i])
    {
        if (s[i] == '.')
        {
            nb_dot++;
            if (nb_dot > 1)
                return 0;
        }
        else if (!ft_isdigit(s[i]))
            return 0;
        nb_digit++;
        i++;
    }
    return (nb_digit > 0 && nb_dot <= 1);
}

float ft_atof(const char *s)
{
    int i;
    float res;
    float fraction;
    int n;

    i = 0;
    res = 0.0f;
    fraction = 0.0f;
    n = 1;

    while ((s[i] >= 9 && s[i] <= 13) || s[i] == 32)
        i++;

    if (s[i] == '+' || s[i] == '-') {
        if (s[i] == '-')
            n = -1;
        i++;
    }

    while (ft_isdigit(s[i])) {
        res = res * 10.0f + (s[i] - '0');
        i++;
    }

    if (s[i] == '.') {
        i++;

        float divisor = 10.0f;
        while (ft_isdigit(s[i])) {
            fraction += (s[i] - '0') / divisor;
            divisor *= 10.0;
            i++;
        }
    }
    return n * (res + fraction);
}

void   option_i(t_data *dt)
{
    float  interval       = 0;
    int    interval_us    = 0;
    char   *param         = NULL;

    if (is_activated_option(dt->act_options, 'i'))
    {
        param = ft_strdup(get_option(dt->act_options, 'i')->param);
        if (param == NULL)
            exit_error("Malloc failure.");
        if (ft_isstrfloat(param) == 0)
            exit_error("invalid argument: '%s'\n", param);
        interval = ft_atof(param); // check_result
        interval_us = (int)(interval * 1000000);
        if (interval_us < 2000)
            exit_error("ping: cannot flood; minimal interval allowed for user is 2ms\n");
        else if (interval > 60)
            exit_error("invalid argument: '%s': out of range: 2ms <= value <= 60s\n", param);
        else
            dt->options_params.seq_interval_us = interval_us;
    }
    else
        dt->options_params.seq_interval_us = SEQUENCE_INTERVAL_S * 1000000;
}

void   option_ttl(t_data *dt)
{
    int  ttl    = 0;
    char *param = NULL;

    if (is_activated_option(dt->act_options, 't'))
    {
        param = ft_strdup(get_option(dt->act_options, 't')->param);
        if (param == NULL)
            exit_error("Malloc failure.");
        if (ft_isstrnum(param) == 0)
            exit_error("invalid argument: '%s'\n", param);
        ttl = ft_atoi(param);
        if (ttl < 0 || ttl > 255)
            exit_error("invalid argument: '%s': out of range: 0 <= value <= 255\n", param);
        else
            dt->one_seq.ttl = ft_atoi(param);
    }
}

void    init_options_params(t_data *dt)
{
    option_p(dt);
    option_i(dt);
    option_ttl(dt);
}

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
    dt->one_seq.icmp_seq_nb = 0;
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

void    init_recv_msg(struct msghdr *msg, char *rcv_packet, struct sockaddr_in sockaddr)
{
	char			buffer[512];
	ssize_t			ret;
	struct iovec	iov =
	{
		.iov_base = rcv_packet,
		.iov_len = ICMP_PACKET_LEN
	};
	msg->msg_name = &sockaddr;
	msg->msg_namelen = sizeof(sockaddr);
	msg->msg_iov = &iov;
	msg->msg_iovlen = 1;
	msg->msg_control = buffer;
	msg->msg_controllen = sizeof(buffer);
	msg->msg_flags = 0;

}
