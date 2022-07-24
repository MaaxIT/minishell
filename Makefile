# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/23 15:05:15 by mbennafl          #+#    #+#              #
#    Updated: 2022/07/16 12:56:53 by mpeharpr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Minishell - A simple remake of bash
# Use this makefile to compile the project

# Colors constants
CLEAR			= \033[m
RED				= \033[0;31m
GREEN			= \033[1;32m
YELLOW			= \033[1;33m
BLUE			= \033[0;34m
PURPLE			= \033[1;35m
CYAN			= \033[1;36m

# Directories
INC_DIR			= include
SRC_DIR			= src
OBJ_DIR			= obj

# Files
PROJECT			= SuperShell
NAME			= minishell
SRC				= $(shell ls $(SRC_DIR))
OBJ				= $(SRC:.c=.o)
INC				= $(shell ls $(INC_DIR))

# Compiler options
CC				= gcc
COMP_FLAGS		= -Wall -Wextra -Werror -g
INCLUDE_FLAGS	= -I$(INC_DIR)/
EXT_COMP_FLAGS	= -lreadline
#READLINE_PATH	= /usr/local/Cellar/readline/8.1.2
READLINE_PATH	= ~/.brew/opt/readline

# Bash commands
RM				= rm -rf
MKDIR			= mkdir -p

# Dependencies
OBJ_DEP			= $(addprefix $(OBJ_DIR)/, $(OBJ))
INC_DEP			= $(addprefix $(INC_DIR)/, $(INC))

# Rules
all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DEP)
	@$(MKDIR) $(OBJ_DIR)
	@echo "$(GREEN)Compiling	$(YELLOW)$(shell basename $<)$(CLEAR)"
	@$(CC) -I $(READLINE_PATH)/include $(COMP_FLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(NAME): $(OBJ_DEP) $(INC_DEP)
	@echo "$(BLUE)Building	$(PURPLE)$(NAME)$(CLEAR)"
	@$(CC) $(EXT_COMP_FLAGS) -L $(READLINE_PATH)/lib $(COMP_FLAGS) $(OBJ_DEP) -o $(NAME)
	@echo "$(GREEN)Program $(PURPLE)($(NAME))$(GREEN) has been successfully generated!$(CLEAR)"

clean:
	@echo "$(RED)Removing	$(PURPLE)$(PROJECT) $(YELLOW)*.o$(CLEAR)"
	@$(RM) $(OBJ_DIR)

fclean: clean
	@echo "$(RED)Removing	$(PURPLE)$(NAME)$(CLEAR)"
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re