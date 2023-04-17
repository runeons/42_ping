/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsantoni <tsantoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/18 09:29:11 by tsantoni          #+#    #+#             */
/*   Updated: 2021/05/18 13:20:53 by ssingevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libftprintf.h>

void	ft_putchar(t_data *current, char c)
{
	write(current->fd, &c, 1);
}

void	ft_putchar_count(t_data *current, char c)
{
	write(current->fd, &c, 1);
	current->count += 1;
}

void	ft_putnbr(t_data *current, int nb)
{
	int	div;

	div = 1;
	if (nb < 0)
	{
		ft_putchar(current, '-');
		nb *= -1;
	}
	while ((nb / div) >= 10)
		div *= 10;
	while (div > 0)
	{
		ft_putchar(current, (nb / div) % 10 + '0');
		div /= 10;
	}
}

void	ft_putnbr_l(t_data *current, long nb)
{
	int	div;

	div = 1;
	if (nb < 0)
	{
		ft_putchar(current, '-');
		nb *= -1;
	}
	while ((nb / div) >= 10)
		div *= 10;
	while (div > 0)
	{
		ft_putchar(current, (nb / div) % 10 + '0');
		div /= 10;
	}
}

void	ft_putnbr_u(t_data *current, unsigned int nb)
{
	int	div;

	div = 1;
	while ((nb / div) >= 10)
		div *= 10;
	while (div > 0)
	{
		ft_putchar(current, (nb / div) % 10 + '0');
		div /= 10;
	}
}
