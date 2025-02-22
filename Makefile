
# Standard
NAME			=	minishell

# Directories
LIBFT_DIR		= 	./libft
SRC_DIR			=	./src

BNF_DIR			=	bnf_syntax
LEX_DIR			=	lexer
PARS_DIR		=	parser
BUILT_DIR		=	builtins
EXEC_DIR		=	execution
REDIR_DIR		=	redirection
ENVP_DIR		=	envp
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
LDFLAGS			=	-lreadline
RM				=	rm -f

# Source files
BNF_FILES		=	input_validation.c \
					pipeline.c \
					command.c \
					simple_command.c \
					redirect.c \
					word.c \
					main.c
#DELETE main.c!
LEX_FILES		=	tokenization.c \
					token_creation.c \
					token_cleaning.c \
					extraction.c \
					skipping.c \
					merging.c \
					redirection.c \
					token_utils.c \
					main.c
#DELETE main.c!
PARS_FILES		=	cmd_creation.c \
					cmd_blocks.c \
					cmd_cleaning.c \
					split_bonus.c \
					main.c
BUILT_FILES		=	cd.c \
					cd2.c \
					cd_error.c \
					env.c \
					export.c \
					export2.c \
					export3.c \
					echo.c \
					exit.c \
					exit2.c \
					pwd.c \
					unset.c \
					unset2.c \
					key_handling.c \
					helper_functions.c
#rename many of them!
EXEC_FILES		=	builtin_check.c \
					cmd_exec.c \
					execute_cmd.c \
					find_path_and_execute_command.c \
					pipex_bonus.c \
					path_bonus.c \
					cmd_errormsg_free.c
REDIR_FILES		=	open_bonus.c \
					fds.c
ENVP_FILES		=	check_all_tokens.c \
					handle_expansion.c \
					initialize_struct.c \
					list.c \
					letters.c
#delete initialize_struct
#SIGN_FILES		=	
CLEAN_FILES		=	arr_cleaning.c \
					double_array_handling.c
#put in one file?
ERR_FILES		=	errors_bonus.c \

SRC_FILES		=	$(addprefix $(BNF_DIR)/, $(BNF_FILES)) \
					$(addprefix $(LEX_DIR)/, $(LEX_FILES)) \
					$(addprefix $(PARS_DIR)/, $(PARS_FILES)) \
					$(addprefix $(EXEC_DIR)/, $(EXEC_FILES)) \
					$(addprefix $(REDIR_DIR)/, $(REDIR_FILES)) \
					$(addprefix $(ENVP_DIR)/, $(ENVP_FILES)) \
					$(addprefix $(CLEAN_DIR)/, $(CLEAN_FILES)) \
					$(addprefix $(ERR_DIR)/, $(ERR_FILES)) \
					$(addprefix $(BUILT_DIR)/, $(BUILT_FILES)) \
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
					@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(HEADERS) $(LIB) $(LDFLAGS) -o $(NAME)
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
