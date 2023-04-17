/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsantoni <tsantoni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 14:59:24 by tsantoni          #+#    #+#             */
/*   Updated: 2021/05/18 12:33:26 by ssingevi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include <stdint.h>

typedef struct s_data {
	int		fd;
	int		fmt_i;
	char	conv;
	int		conv_i;
	int		align;
	int		width;
	int		zero;
	int		prec;
	int		neg_sign;
	int		count;
	char	gap;
}				t_data;

int				ft_display(t_data *current, va_list arg_val);
int				ft_printf(const char *fmt, ...);
int				ft_dprintf(int fd, const char *fmt, ...);

void			ft_putchar(t_data *current, char c);
void			ft_putchar_count(t_data *current, char c);
void			ft_putnbr(t_data *current, int nb);
void			ft_putnbr_l(t_data *current, long nb);
void			ft_putnbr_u(t_data *current, unsigned int nb);

void			ft_puts(t_data *current, char *str);
int				ft_strlen_pf(char *str);
int				ft_is_digit(char c);
char			*ft_strdup_pf(char *str);
char			*ft_strndup_pf(char *str, int n);

int				ft_is_token(char token, char tocheck);
void			ft_parse_gap(t_data *current);
void			ft_display_gap(t_data *current, int tofill);
void			ft_display_zero(t_data *current, int tofill);

void			ft_init_data(t_data *current, int fd);
void			ft_reinit_data(t_data *current);
void			ft_fill_data(t_data *current, const char *fmt, va_list arg_val);

char			ft_find_conv(t_data *current, const char *fmt);
void			ft_parse_align(t_data *current, const char *fmt);
void			ft_parse_zero(t_data *current, const char *fmt);
void			ft_parse_prec(t_data *current, const char *fmt, va_list av);
void			ft_parse_width(t_data *current, const char *fmt, va_list av);

void			ft_display_c(t_data *current, va_list arg_val);

void			ft_display_s(t_data *current, va_list arg_val);

void			ft_update_zero(t_data *current);
int				ft_digits_count_u(unsigned int nb);
int				ft_display_u(t_data *current, va_list arg_val);

int				ft_digits_count_l(long nb);
void			ft_display_d(t_data *current, long b_arg, int not_gp, int nb_d);
long			ft_change_sign(t_data *current, long buf_arg);
int				ft_prepare_d(t_data *current, va_list arg_val);
int				ft_define_not_gap(t_data *current, int nb_digits);

char			*ft_itoa_base_ui(unsigned int nb, char *base, int base_len);
void			ft_display_x(t_data *current, int nt_gp, char *ba_h, int nb_d);
int				ft_prepare_x(t_data *current, va_list arg_val, char *base);

char			*ft_itoa_base_llu(uint64_t nb, char *base, int base_len);
void			ft_display_p(t_data *current, char *ba_h, int nt_gp, int nb_d);
int				ft_prepare_p(t_data *current, va_list arg_val, char *base);

void			ft_display_pc(t_data *current);
#endif
