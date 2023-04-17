/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conv_u.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsantoni <tsantoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/16 14:09:02 by tsantoni          #+#    #+#             */
/*   Updated: 2021/05/18 12:35:54 by ssingevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libftprintf.h>

void	ft_update_zero(t_data *current)
{
	if (current->prec != -1)
		current->zero = 0;
	if (current->align == 1)
		current->zero = 0;
	ft_parse_gap(current);
}

int	ft_digits_count_u(unsigned int nb)
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

int	ft_display_u(t_data *current, va_list arg_val)
{
	int				not_gap;
	unsigned int	buf_arg;
	int				nb_digits;

	buf_arg = va_arg(arg_val, unsigned int);
	nb_digits = ft_digits_count_u(buf_arg);
	not_gap = nb_digits;
	ft_update_zero(current);
	if (current->prec == 0 && buf_arg == 0)
	{
		ft_display_gap(current, current->width);
		return (0);
	}
	if (current->prec > nb_digits)
		not_gap = current->prec;
	if (current->align == 0)
		ft_display_gap(current, current->width - not_gap);
	ft_display_zero(current, not_gap - nb_digits);
	ft_putnbr_u(current, buf_arg);
	current->count += nb_digits;
	if (current->align == 1)
		ft_display_gap(current, current->width - not_gap);
	return (0);
}
