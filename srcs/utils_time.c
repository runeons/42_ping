#include "ping_functions.h"

int    get_time_min(t_data *dt)
{
    int min_time = MAX_INT;
    int *tmp = NULL;

    t_lst *current = dt->end_stats.times;
    while (current != NULL)
    {
        tmp = (int *)(current->content);
        if (tmp != NULL && *tmp < min_time)
            min_time = *tmp;
        current = current->next;
    }
    return (min_time);
}

int    get_time_max(t_data *dt)
{
    int max_time = MIN_INT;
    int *tmp = NULL;

    t_lst *current = dt->end_stats.times;
    while (current != NULL)
    {
        tmp = (int *)(current->content);
        if (tmp != NULL && *tmp > max_time)
            max_time = *tmp;
        current = current->next;
    }
    return (max_time);
}

int    get_time_sum(t_data *dt)
{
    int time_sum = 0;
    int *tmp = NULL;

    t_lst *current = dt->end_stats.times;
    while (current != NULL)
    {
        tmp = (int *)(current->content);
        if (tmp != NULL)
            time_sum += *tmp;
        current = current->next;
    }
    return (time_sum);
}

int    get_time_avg(t_data *dt)
{
    int time_avg = 0;
    
    if (dt != NULL && ft_lst_size(dt->end_stats.times))
        time_avg = get_time_sum(dt) / ft_lst_size(dt->end_stats.times);
    return (time_avg);
}

int    get_time_std(t_data *dt)
{
    int variance = 0;
    int std = 0;
    int *tmp = NULL;
    int mean = get_time_avg(dt);

    t_lst *current = dt->end_stats.times;
    while (current != NULL)
    {
        tmp = (int *)(current->content);
        if (tmp != NULL)
            variance += ((*tmp - mean) * (*tmp - mean));
        current = current->next;
    }
    if (ft_lst_size(dt->end_stats.times) != 0)
        std = ft_sqrt(variance / ft_lst_size(dt->end_stats.times));
    return (std);
}
