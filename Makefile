# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: azinchen <azinchen@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/07 20:16:32 by azinchen          #+#    #+#              #
#    Updated: 2024/11/07 20:53:26 by azinchen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Standard
NAME			=	pipex

# Directories
LIBFT_DIR		= 	./libft
SRC_DIR			=	./src
SRC_MAN_DIR		=	$(SRC_DIR)/mandatory
SRC_BON_DIR		=	$(SRC_DIR)/bonus
OBJ_DIR			=	./obj
OBJ_MAN_DIR 	=	$(OBJ_DIR)/mandatory
OBJ_BON_DIR		=	$(OBJ_DIR)/bonus

#Includes
LIBFT_INC		=	$(LIBFT_DIR)/include
HEADERS			=	-I$(LIBFT_INC)

#Libraries
LIBFT			=	$(LIBFT_DIR)/libft.a
LIB				=	-L$(LIBFT_DIR)

# Compiler and compilation flags
CC				=	cc
CFLAGS			=	-g -Wall -Wextra -Werror
RM				=	rm -f

# Source files
MAN_SRC			=	pipex.c \
					path.c \
					errors.c \
					checks.c \
					cleaners.c \
					split.c \
					open.c \
					utils.c
BON_SRC			=	pipex_bonus.c \
					path_bonus.c \
					errors_bonus.c \
					checks_bonus.c \
					cleaners_bonus.c \
					split_bonus.c \
					open_bonus.c \
					utils_bonus.c

# Full paths to files
MAN_SRC_FILES	=	$(addprefix $(SRC_MAN_DIR)/, $(MAN_SRC))
BON_SRC_FILES	=	$(addprefix $(SRC_BON_DIR)/, $(BON_SRC))

# Object files
MAN_OBJ			=	$(addprefix $(OBJ_MAN_DIR)/, $(MAN_SRC:.c=.o))
BON_OBJ			=	$(addprefix $(OBJ_BON_DIR)/, $(BON_SRC:.c=.o))

# Colors
BLUE			=	\033[1;34m
GREEN			=	\033[1;32m
NC				=	\033[0m

# Build rules
all:				$(NAME)

$(LIBFT):			
					@make -C $(LIBFT_DIR)

$(NAME):			.mandatory

.mandatory:			$(MAN_OBJ) $(LIBFT)
					@$(CC) $(CFLAGS) $(MAN_OBJ) $(LIBFT) $(HEADERS) $(LIB) -o $(NAME)
					@touch $@
					@echo "$(GREEN)--> Created pipex!$(NC)"
					@$(RM) -r .bonus

bonus:				.bonus

.bonus:				$(BON_OBJ) $(LIBFT)
					@$(CC) $(CFLAGS) $(BON_OBJ) $(LIBFT) $(HEADERS) $(LIB) -o $(NAME)
					@touch $@
					@echo "$(GREEN)--> Created pipex_bonus!$(NC)"
					@$(RM) -r .mandatory

# Object file compilation
$(OBJ_MAN_DIR)/%.o: $(SRC_MAN_DIR)/%.c
					@mkdir -p $(@D)
					@$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

$(OBJ_BON_DIR)/%.o: $(SRC_BON_DIR)/%.c
					@mkdir -p $(@D)
					@$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

# Cleaning rules
clean:
					@$(RM) -r $(OBJ_DIR)
					@$(RM) .cache_exists
					@make clean -C $(LIBFT_DIR)
					@echo "$(BLUE)*.o files removed!$(NC)"
		
fclean:				clean 
					@$(RM) $(NAME)
					@$(RM) $(LIBFT)
					@$(RM) -r .mandatory .bonus
					@echo "$(BLUE)All files removed!$(NC)"
			
re:					fclean all

.PHONY:				all clean fclean re bonus
