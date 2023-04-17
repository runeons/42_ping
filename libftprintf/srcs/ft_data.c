/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_data.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsantoni <tsantoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 15:21:32 by tsantoni          #+#    #+#             */
/*   Updated: 2021/05/18 13:18:17 by ssingevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libftprintf.h>

void	ft_init_data(t_data *current, int fd)
{
	current->fd = fd;
	current->fmt_i = -1;
	current->conv = '\0';
	current->conv_i = 0;
	current->align = 0;
	current->width = 0;
	current->zero = 0;
	current->prec = 0;
	current->neg_sign = 0;
	current->count = 0;
	current->gap = '\0';
}

void	ft_reinit_data(t_data *current)
{
	current->align = 0;
	current->width = -1;
	current->zero = 0;
	current->prec = -1;
	current->neg_sign = 0;
	current->gap = '\0';
}

void	ft_fill_data(t_data *current, const char *fmt, va_list arg_val)
{
	current->conv = ft_find_conv(current, fmt);
	ft_parse_align(current, fmt);
	ft_parse_zero(current, fmt);
	ft_parse_width(current, fmt, arg_val);
	ft_parse_prec(current, fmt, arg_val);
	ft_parse_gap(current);
}
