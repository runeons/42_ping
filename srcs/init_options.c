#include "ping_functions.h"

void   option_v(t_data *dt)
{
    if (is_activated_option(dt->act_options, 'v'))
        dt->options_params.v = 1;
    else
        dt->options_params.v = 0;
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
            exit_error("ping: malloc failure.\n");
        if (ft_isstrfloat(param) == 0)
            exit_error("ping: invalid value (`%s' near `%s')\nTry 'ping --help' or 'ping --usage' for more information.\n", param, param);
        interval = ft_atof(param);
        interval_us = (int)(interval * 1000000);
        if (interval_us < 200000)
            exit_error("ping: option value too small: %s\n", param);
        else if (interval > 2048)
            exit_error("ping: option value too big: %s\n", param);
        else
            dt->options_params.seq_interval_us = interval_us;
    }
    else
        dt->options_params.seq_interval_us = SEQUENCE_INTERVAL_S * 1000000;
}

void   option_c(t_data *dt)
{
    int    count          = 0;
    char   *param         = NULL;

    if (is_activated_option(dt->act_options, 'c'))
    {
        param = ft_strdup(get_option(dt->act_options, 'c')->param);
        if (param == NULL)
            exit_error("ping: malloc failure.\n");
        if (ft_isstrnum(param) == 0)
            exit_error("ping: invalid value: (`%s' near `%s')\n", param, param);
        count = ft_atoi(param);
        if (count < 1 || count > MAX_INT)
        {
            dt->options_params.count = 0;
            if (dt->options_params.v)
                warning_error("ping: warning: '%s' ignored as out of range: 1 <= value <= MAX_INT\n", param);
        }
        else
            dt->options_params.count = count;
    }
    else
        dt->options_params.count = 0;
}

void   option_w(t_data *dt)
{
    int    timeout        = 0;
    char   *param         = NULL;

    if (is_activated_option(dt->act_options, 'w'))
    {
        param = ft_strdup(get_option(dt->act_options, 'w')->param);
        if (param == NULL)
            exit_error("ping: malloc failure.\n");
        if (ft_isstrnum(param) == 0)
            exit_error("ping: invalid value: (`%s' near `%s')\n", param, param);
        timeout = ft_atoi(param);
        if (timeout < 1 || timeout > MAX_INT)
        {
            dt->options_params.w_timeout = 0;
            if (dt->options_params.v)
                warning_error("ping: warning: '%s' ignored as out of range: 1 <= value <= MAX_INT\n", param);
        }
        else
            dt->options_params.w_timeout = timeout;
    }
    else
        dt->options_params.w_timeout = 0;
}

static int hex_pos(char a)
{
    for (int i = 0; i < 16; i++)
    {
        if (a == HEX_BASE_U[i])
            return i;
        if (a == HEX_BASE_L[i])
            return i;
    }
    return -1;
}

void   option_p(t_data *dt)
{
    if (is_activated_option(dt->act_options, 'p'))
    {
        int param_len = ft_cap(ft_strlen(get_option(dt->act_options, 'p')->param), 16);

        char *param = ft_strndup(get_option(dt->act_options, 'p')->param, param_len);
        if (ft_ishex(param, param_len) == 0)
            exit_error("ping: error in pattern near %s\n", param);
        for (int i = 0; i < ICMP_DATA_LEN * 2; i += 2)
            dt->options_params.p_payload[i / 2] = (hex_pos(param[i % param_len]) << 4) | hex_pos(param[(i + 1) % param_len]);
    }
    else
    {
        for (int i = 0; i < ICMP_DATA_LEN; i++)
            dt->options_params.p_payload[i] = i;
        dt->options_params.p_payload[ICMP_DATA_LEN] = '\0';
    }
}

void   option_ttl(t_data *dt)
{
    int  ttl    = 0;
    char *param = NULL;

    if (is_activated_option(dt->act_options, 't'))
    {
        param = ft_strdup(get_option(dt->act_options, 't')->param);
        if (param == NULL)
            exit_error("ping: malloc failure.\n");
        if (ft_isstrnum(param) == 0)
            exit_error("ping: invalid value: (`%s' near `%s')\n", param, param);
        ttl = ft_atoi(param);
        if (ttl <= 0)
            exit_error("ping: option value too small: %d\n", ttl);
        else if (ttl > 255)
            exit_error("ping: option value too big: %d\n", ttl);
        else
            dt->one_seq.ttl = ft_atoi(param);
    }
    else
        dt->one_seq.ttl = TTL_VALUE;
}

void    init_options_params(t_data *dt)
{
    option_v(dt);
    option_p(dt);
    option_c(dt);
    option_w(dt);
    option_i(dt);
    option_ttl(dt);
}
