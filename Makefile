
# Standard
NAME			=	minishell

# Directories
LIBFT_DIR		= 	./libft
SRC_DIR			=	./src

BNF_DIR			=	bnf_syntax
PARS_DIR		=	parsing
BUILT_DIR		=	builtins
EXEC_DIR		=	execution
REDIR_DIR		=	redirection
SIGN_DIR		=	signals
CLEAN_DIR		=	cleaners
ERR_DIR			=	errors

OBJ_DIR			=	./obj

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
BNF_FILES		=	input_validation.c \
					and_or.c \
					pipeline.c \
					command.c \
					simple_command.c \
					redirect.c \
					word.c
PARS_FILES		=	split_bonus.c
#BUILT_FILES	=	
EXEC_FILES		=	pipex_bonus.c \
					path_bonus.c \
					checks_bonus.c \
					utils_bonus.c
REDIR_FILES	=		open_bonus.c \
#SIGN_FILES		=	
CLEAN_FILES		=	cleaners_bonus.c
ERR_FILES		=	errors_bonus.c \

SRC_FILES		=	$(addprefix $(PARS_DIR)/, $(PARS_FILES)) \
					$(addprefix $(EXEC_DIR)/, $(EXEC_FILES)) \
					$(addprefix $(REDIR_DIR)/, $(REDIR_FILES)) \
					$(addprefix $(CLEAN_DIR)/, $(CLEAN_FILES)) \
					$(addprefix $(ERR_DIR)/, $(ERR_FILES)) \
					#$(addprefix $(BUILT_DIR)/, $(BUILT_FILES)) \
					$(addprefix $(SIGN_DIR)/, $(SIGN_FILES)) \

# Full paths to files
SRC				=	$(addprefix $(SRC_DIR)/, $(SRC_FILES))

# Object files
OBJ				=	$(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC:.c=.o))

# Colors
BLUE			=	\033[1;34m
GREEN			=	\033[1;32m
NC				=	\033[0m

# Build rules
all:				$(NAME)

$(LIBFT):			
					@make -C $(LIBFT_DIR)

$(NAME):			.build

.build:				$(OBJ) $(LIBFT)
					@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(HEADERS) $(LIB) -o $(NAME)
					@touch $@
					@echo "$(GREEN)--> Created minishell!$(NC)"

# Object file compilation
$(OBJ_DIR)/%.o: 	$(SRC_DIR)/%.c
					@mkdir -p $(@D)
					@$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

# Cleaning rules
clean:
					@$(RM) -r $(OBJ_DIR)
					@make clean -C $(LIBFT_DIR)
					@echo "$(BLUE)*.o files removed!$(NC)"
		
fclean:				clean 
					@$(RM) $(NAME)
					@$(RM) $(LIBFT)
					@$(RM) -r .build
					@echo "$(BLUE)All files removed!$(NC)"
			
re:					fclean all

.PHONY:				all clean fclean re
