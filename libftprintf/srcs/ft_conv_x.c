/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conv_x.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsantoni <tsantoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/17 17:23:00 by tsantoni          #+#    #+#             */
/*   Updated: 2021/05/18 12:36:28 by ssingevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libftprintf.h>

char	*ft_itoa_base_ui(unsigned int nb, char *base, int base_len)
{
	char			*res;
	int				i;
	unsigned int	div;
	unsigned int	temp;

	i = 1;
	div = 1;
	temp = nb;
	while (nb >= (unsigned int)base_len)
	{
		nb /= base_len;
		div *= base_len;
		i++;
	}
	res = malloc(sizeof(char) * i + 1);
	if (!(res))
		return (NULL);
	i = 0;
	while (div > 0)
	{
		res[i++] = base[(temp / div) % base_len];
		div /= base_len;
	}
	res[i] = '\0';
	return (res);
}

void	ft_display_x(t_data *current, int nt_gp, char *ba_h, int nb_d)
{
	if (current->align == 0)
		ft_display_gap(current, current->width - nt_gp);
	ft_display_zero(current, nt_gp - nb_d);
	ft_puts(current, ba_h);
	current->count += nb_d;
	if (current->align == 1)
		ft_display_gap(current, current->width - nt_gp);
}

int	ft_prepare_x(t_data *current, va_list arg_val, char *base)
{
	int				not_gap;
	unsigned int	buf_arg;
	char			*buf_arg_hex;
	int				nb_digits;

	buf_arg = va_arg(arg_val, unsigned int);
	buf_arg_hex = ft_itoa_base_ui(buf_arg, base, 16);
	nb_digits = ft_strlen_pf(buf_arg_hex);
	not_gap = nb_digits;
	ft_update_zero(current);
	if (current->prec == 0 && buf_arg == 0)
	{
		ft_display_gap(current, current->width);
		free(buf_arg_hex);
		return (0);
	}
	if (current->prec > nb_digits)
		not_gap = current->prec;
	ft_display_x(current, not_gap, buf_arg_hex, nb_digits);
	free(buf_arg_hex);
	return (0);
}
