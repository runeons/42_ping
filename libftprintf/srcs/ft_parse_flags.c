/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_flags.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsantoni <tsantoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/11 15:06:27 by tsantoni          #+#    #+#             */
/*   Updated: 2021/05/18 13:11:32 by ssingevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libftprintf.h>

char	ft_find_conv(t_data *current, const char *fmt)
{
	char	*all_conv;
	int		i;
	int		j;

	all_conv = "cspdiuxX%";
	i = current->fmt_i + 1;
	while (fmt[i])
	{
		j = 0;
		while (all_conv[j])
		{
			if (fmt[i] == all_conv[j])
			{
				current->conv_i = i;
				return (all_conv[j]);
			}
			else
				j++;
		}
		i++;
	}
	current->conv_i = i;
	return ('\0');
}

void	ft_parse_align(t_data *current, const char *fmt)
{
	int	i;

	i = current->fmt_i + 1;
	while (fmt[i] != current->conv)
	{
		if (fmt[i] == '-')
			current->align = 1;
		i++;
	}
}

void	ft_parse_zero(t_data *current, const char *fmt)
{
	int	i;

	i = current->fmt_i + 1;
	while (fmt[i] != current->conv)
	{
		if (fmt[i] == '0' && ft_is_digit(fmt[i - 1]) == 0 && fmt[i - 1] != '.')
			current->zero = 1;
		i++;
	}
}

void	ft_parse_prec(t_data *current, const char *fmt, va_list av)
{
	int	i;
	int	buf_arg;

	i = current->fmt_i;
	while (fmt[++i] != current->conv)
	{
		if (fmt[i] == '.' && ft_is_digit(fmt[i + 1]) == 0
			&& ft_is_token('*', fmt[i + 1]) == 1)
		{
			buf_arg = va_arg(av, int);
			current->prec = buf_arg;
			if ((buf_arg) < 0)
				current->prec = -1;
		}
		else if (fmt[i] == '.' && ft_is_digit(fmt[i + 1]) == 0
			&& !(ft_is_token('*', fmt[i + 1]) == 1))
			current->prec = 0;
		else if (fmt[i] == '.')
		{
			current->prec = 0;
			while (ft_is_digit(fmt[i + 1]) == 1)
				current->prec = current->prec * 10 + (fmt[i++ + 1] - '0');
		}
	}
}

void	ft_parse_width(t_data *current, const char *fmt, va_list av)
{
	int	i;
	int	buf_arg;

	i = current->fmt_i;
	while (fmt[++i] != current->conv)
	{
		if (ft_is_digit(fmt[i]) == 1 && fmt[i - 1] != '.'
			&& ft_is_digit(fmt[i - 1]) == 0
			&& !(fmt[i] == '0' && ft_is_digit(fmt[i + 1]) != 1))
		{
			current->width = fmt[i] - '0';
			while (ft_is_digit(fmt[i + 1]) == 1)
				current->width = current->width * 10 + (fmt[i++ + 1] - '0');
		}
		if (ft_is_token('*', fmt[i]) == 1 && fmt[i - 1] != '.')
		{
			buf_arg = va_arg(av, int);
			current->width = buf_arg;
			if (buf_arg < 0)
			{
				current->align = 1;
				current->width = -buf_arg;
			}
		}
	}
}
