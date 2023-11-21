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
