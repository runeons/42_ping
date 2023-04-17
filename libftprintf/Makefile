NAME 					= libftprintf.a

CC 						= clang
CFLAGS 					= -Wall -Wextra -Werror

HDIR					=	$(addprefix -I, $(HEADER_DIR))

HEAD					=	$(addprefix $(HEADER_DIR), $(HEADER))

HEADER_DIR				=	\
							./includes/

HEADER							=	\
							libftprintf.h \


SRCS_DIR		=./srcs/

SRCS_LIST				=	\
					ft_printf.c \
					ft_printf_utils.c \
					ft_data.c \
					ft_parse_flags.c \
					ft_libft.c \
					ft_conv_s.c \
					ft_conv_c.c \
					ft_conv_u.c \
					ft_conv_d.c \
					ft_conv_x.c \
					ft_conv_p.c \
					ft_conv_pc.c \
					ft_put.c \

SRCS					=	$(addprefix $(SRCS_DIR), $(SRCS_LIST))

OBJS_DIR				=	./objs/

OBJS_LIST				=	$(patsubst %.c, %.o, $(SRCS_LIST))

OBJS					=	$(addprefix $(OBJS_DIR), $(OBJS_LIST))

OPTION = -c -I .

all: $(OBJS) $(SRCS) $(NAME)

$(NAME): $(OBJS) $(SRCS)
	@ ar rcs $(NAME) $(OBJS)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) $(HDIR) $(OPTION) -o $@ $<

clean:
	@ rm -rf $(OBJS_DIR)

fclean: clean
	@ rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean
