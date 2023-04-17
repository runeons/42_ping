# *** NAME - CC - FLAGS ****************************************************** #

NAME					= 	ft_ping

CC						= 	clang

CFLAGS					= 	-Wall -Wextra -Werror -I includes/ \
							-I libft/includes/ \

BONUS_FLAG				= 	0

TESTER_FLAG				= 	0

INC_LIB						= 	-I libft
LIB							= 	-L libft -lft



# *** PROJECT HEADER ********************************************************* #

HDIR					=	$(addprefix -I, $(HEADER_DIR))

HEAD					=	$(addprefix $(HEADER_DIR), $(HEADER))

HEADER_DIR				=	\
							./includes/

HEADER							=	\
									color_shell.h \
									ft_ping.h \


# *** SRCS ******************************************************************* #

SRCS_DIR					=	./srcs/

SRCS_LIST					=	\
							main.c \
							# $(UTILS) \
							# $(MAIN) \

# UTILS						=	\
# 							utils/signals.c \
# 							utils/utils.c \

# MAIN						=	\
# 							main/env.c \
# 							main/handle_keys.c \
# 							main/history.c \
# 							main/main.c \
# 							main/read_input.c \

SRCS					=	$(addprefix $(SRCS_DIR), $(SRCS_LIST))

# *** OBJS ******************************************************************* #

OBJS_DIR				=	./objs/

OBJS_LIST				=	$(patsubst %.c, %.o, $(SRCS_LIST))

OBJS					=	$(addprefix $(OBJS_DIR), $(OBJS_LIST))



all: $(OBJS) $(SRCS) $(NAME)

$(NAME): $(OBJS) $(SRCS)
	@ make -C libft/
	@ $(CC) $(CFLAGS) $(HDIR) $(OBJS) $(INC_LIB) $(LIB) -o $@


$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@ mkdir -p $(dir $@)
	@ $(CC) $(CFLAGS) $(HDIR) -D BONUS=$(BONUS_FLAG) $(INC_LIB) -c -o $@ $<

clean:
	make clean -C libft/
	rm -rf $(OBJS_DIR)

fclean:
	make fclean -C libft/
	rm -f $(NAME)
	rm -rf $(OBJS_DIR)

re: fclean all

bonus:
	make fclean
	make -j BONUS_FLAG=1

.PHONY: all clean fclean re bonus
