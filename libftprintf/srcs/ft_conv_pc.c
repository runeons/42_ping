/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conv_pc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsantoni <tsantoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/17 21:25:18 by tsantoni          #+#    #+#             */
/*   Updated: 2021/03/01 13:57:38 by tharchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libftprintf.h>

void	ft_display_pc(t_data *current)
{
	if (current->align == 0)
		ft_display_gap(current, current->width - 1);
	ft_putchar(current, '%');
	current->count += 1;
	if (current->align == 1)
		ft_display_gap(current, current->width - 1);
}
