#ifndef UTILS_OPTIONS_H
# define UTILS_OPTIONS_H

# include "stdio.h"
# include <libft.h>

# define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0])) // int option_count = ARRAY_SIZE(allowed_options);

typedef struct s_parsed_cmd
{
    t_lst *act_options;
    t_lst *not_options;
}              t_parsed_cmd;

typedef struct s_option
{
    char    id;
    char    *name;
    int     need_param;
    char    *description;
    char    *param;
}              t_option;

void            display_help();
void            debug_activated_options(t_lst *act_options);
int             is_activated_option(t_lst *act_options, char c);
t_parsed_cmd    parse_options(int ac, char **av);
t_option        *get_option(t_lst *act_options, char c);
void            print_option(t_lst *act_options, char c);

#endif