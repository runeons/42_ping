/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conv_d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsantoni <tsantoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/17 16:33:44 by tsantoni          #+#    #+#             */
/*   Updated: 2021/05/18 12:34:46 by ssingevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libftprintf.h>
#include <stdio.h>

int	ft_digits_count_l(long nb)
{
	int		nb_digits;

	nb_digits = 0;
	if (nb == 0)
		return (1);
	while (nb > 0)
	{
		nb /= 10;
		nb_digits++;
	}
	return (nb_digits);
}

void	ft_display_d(t_data *current, long b_arg, int not_gp, int nb_d)
{
	if (current->align == 0)
		ft_display_gap(current, current->width - not_gp);
	if (current->neg_sign == 1)
		ft_putchar_count(current, '-');
	if (not_gp > nb_d)
		ft_display_zero(current, not_gp - nb_d - current->neg_sign);
	ft_putnbr_l(current, b_arg);
	current->count += nb_d;
	if (current->align == 1)
		ft_display_gap(current, current->width - not_gp);
}

long	ft_change_sign(t_data *current, long buf_arg)
{
	current->neg_sign = 1;
	return (-buf_arg);
}

int	ft_prepare_d(t_data *current, va_list arg_val)
{
	int		buf_arg_tmp;
	long	buf_arg;
	int		not_gap;
	int		nb_digits;

	ft_update_zero(current);
	buf_arg_tmp = va_arg(arg_val, int);
	buf_arg = (long)buf_arg_tmp;
	if (buf_arg < 0)
		buf_arg = ft_change_sign(current, buf_arg);
	nb_digits = ft_digits_count_l(buf_arg);
	if (current->prec == 0 && buf_arg == 0)
	{
		ft_display_gap(current, current->width);
		return (0);
	}
	not_gap = nb_digits + current->neg_sign;
	if (current->prec == -1 && current->zero == 1 && current->width > not_gap)
		current->prec = current->width - current->neg_sign;
	if (current->prec >= not_gap)
		not_gap = current->prec + current->neg_sign;
	ft_display_d(current, buf_arg, not_gap, nb_digits);
	return (0);
}
