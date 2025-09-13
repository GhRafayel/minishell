# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rghazary <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/14 12:41:29 by rghazary          #+#    #+#              #
#    Updated: 2025/04/26 11:03:23 by rghazary         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = minishell.c print.c file.c execute.c cd.c \
      $(P_T)string.c \
      $(P_T)free.c \
	  $(P_T)utils_1.c \
      $(P_T)env_list.c \
      $(P_T)token_list.c \
      $(P_T)expansion.c \
      $(P_T)quote.c \
      $(P_T)utils.c \
      $(P_T)init.c \
      $(P_T)redirect.c \
	  $(P_T)cmd.c \
	  $(P_T)signal.c \
	  $(B_I)heredoc.c \
	  $(B_I)app_inp_out.c \
	  $(B_I)export.c \
	  $(B_I)unset.c \
	  $(B_I)exit.c \
	  $(B_I)echo_env_pwd.c \
	  $(B_I)utils.c \

NAME = minishell
P_T = parsing_and_tokenizing/
B_I = builtIn/
CC = cc
FLAGS = -Wall -Wextra -Werror -MMD
LIB_DIR = src/libft
LIB = $(LIB_DIR)/libft.a
LDFLAGS = -lreadline -lncurses
OBJ = $(SRC:%.c=%.o)
DEP = $(OBJ:.o=.d)

all: $(NAME)
	$(MAKE) -C $(LIB_DIR)

$(NAME): $(OBJ) $(LIB)
	$(CC) $(FLAGS) $(OBJ) $(LIB) -o $(NAME) $(LDFLAGS)

$(LIB):
	$(MAKE) -C $(LIB_DIR)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(DEP)
	$(MAKE) -C $(LIB_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIB_DIR) fclean

re: fclean all

-include $(DEP)

.PHONY: all clean fclean re
