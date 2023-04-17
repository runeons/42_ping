/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsantoni <tsantoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 15:04:46 by tsantoni          #+#    #+#             */
/*   Updated: 2021/05/18 13:20:24 by ssingevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libftprintf.h>

int	ft_is_token(char token, char tocheck)
{
	if (tocheck == token)
		return (1);
	return (0);
}

void	ft_parse_gap(t_data *current)
{
	if (current->align == 1)
		current->gap = ' ';
	else
	{
		if (current->zero == 0)
			current->gap = ' ';
		else
			current->gap = '0';
	}
}

void	ft_display_gap(t_data *current, int tofill)
{
	int	i;

	i = 0;
	if (tofill > 0)
	{
		while (i < tofill)
		{
			ft_putchar(current, current->gap);
			current->count += 1;
			i++;
		}
	}
}

void	ft_display_zero(t_data *current, int tofill)
{
	int	i;

	i = 0;
	if (tofill > 0)
	{
		while (i < tofill)
		{
			ft_putchar(current, '0');
			current->count += 1;
			i++;
		}
	}
}
