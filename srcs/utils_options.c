#include "utils_options.h"

t_option allowed_options[] =
{
    {'h', "help", 0, "Show help.", NULL},
    {'v', "verbose", 0, "Verbose output. Do not suppress DUP replies when pinging multicast address.", NULL},
    {'V', "version", 0, "Show version and exit.", NULL},
    {'p', "pattern", 1, "You may specify up to 16 pad bytes to fill out the packet you send. This is useful for diagnosing data-dependent problems in a network. For example, -p ff will cause the sent packet to be filled with all ones.", NULL},
};

int _get_name_max_len()
{
    int max_len = 0;
    int curr_len = 0;
    for (int i = 0; i < ARRAY_SIZE(allowed_options); i++)
    {
        curr_len = ft_strlen(allowed_options[i].name) + 4;
        if (curr_len > max_len)
            max_len = curr_len;
    }
    return max_len;
}

void    display_help()
{
    int max_len = _get_name_max_len();
    char formatted_name[max_len + 1];

    printf("Description:\n");
    printf("    ft_ping sends ICMP ECHO_REQUEST to network hosts\n\n");
    printf("Usage:\n");
    printf("    ft_ping {destination ipv4/hostname} ");
    for (int i = 0; i < ARRAY_SIZE(allowed_options); i++)
    {
        if (allowed_options[i].need_param == 1)
            printf("[-%c %s] ", allowed_options[i].id, allowed_options[i].name);
        else if (allowed_options[i].need_param == 0)
            printf("[-%c] ", allowed_options[i].id);
    }
    printf("\n\n");
    printf("Options:\n");
    for (int i = 0; i < ARRAY_SIZE(allowed_options); i++)
    {
        if (allowed_options[i].need_param == 1)
            sprintf(formatted_name, "<%s>", allowed_options[i].name);
        else if (allowed_options[i].need_param == 0)
            sprintf(formatted_name, " ");
        printf("    -%c %-*s %s\n", allowed_options[i].id, max_len, formatted_name, allowed_options[i].description);
    }
    printf("\n");
}

t_option *get_option(t_lst *act_options, char c)
{
    while (act_options != NULL)
    {
        t_option *tmp = (t_option *)act_options->content;
        if (tmp->id == c)
            return tmp;
        act_options = act_options->next;
    }
    return NULL;
}

void    print_option(t_lst *act_options, char c)
{
    t_option *option = get_option(act_options, c);
    if (option->id != 0)
    {
        printf("id: %c\n", option->id);
        printf("name: %s\n", option->name);
        printf("need_param: %d\n", option->need_param);
        printf("description: %s\n", option->description);
        printf("param: %s\n", option->param);
    }
}

void _debug_option(void *content)
{
    if (content)
    {
        t_option *option = (t_option *)content;
        printf("id: %c, name: %s, need_param: %d, param: %s, \n", option->id, option->name, option->need_param, option->param);
    }
}

void    debug_activated_options(t_lst *act_options)
{
    if (act_options)
    {
        printf("\n\n");
        printf("Activated options:\n");
        ft_lst_iter_content(act_options, _debug_option);
    }
}

int is_activated_option(t_lst *act_options, char c)
{
    if (act_options)
    {
        t_lst *current_node = act_options;

        while (current_node)
        {
            t_option *current_option = (t_option *)current_node->content;
            if (current_option->id == c)
                return(1);
			current_node = current_node->next;
        }
    }
    return(0);
}

t_option    *_is_allowed_option(char c)
{
    for (int i = 0; i < ARRAY_SIZE(allowed_options); i++) 
    {
        if (allowed_options[i].id == c)
            return &allowed_options[i];
    }
    return (NULL);
}

t_option *_check_option(char **av, int i)
{
    t_option *res = NULL;
    if (ft_strlen(av[i]) == 2 && (res = _is_allowed_option(av[i][1])) != NULL)
        return res;
    else
    {
        dprintf(2, "invalid option '%s'\n", av[i]);
        exit(1);
    }
}

t_parsed_cmd   parse_options(int ac, char **av)
{
    t_parsed_cmd    result;
    t_lst           *act_options = NULL;
    t_lst           *not_options = NULL;
    t_option        *res = NULL;

    for (int i = 1; i < ac; i++)
    {
        if (ft_strlen(av[i]) && av[i][0] == '-')
        {
            res = _check_option(av, i);
            if (res->need_param)
            {
                if (++i == ac)
                    exit_error("option needs %s\n", res->name);
                res->param = ft_strdup(av[i]);
            }
            ft_lst_add_node_back(&act_options, ft_lst_create_node(res));
        }
        else
            ft_lst_add_node_back(&not_options, ft_lst_create_node(av[i]));
    }
    result.act_options = act_options;
    result.not_options = not_options;
    return result;
}
