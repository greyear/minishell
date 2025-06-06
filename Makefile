
# Standard
NAME			=	minishell

# Directories
LIBFT_DIR		= 	./libft
SRC_DIR			=	./src

MAIN_DIR		=	main
BNF_DIR			=	bnf_syntax
LEX_DIR			=	lexer
PARS_DIR		=	parser
BUILT_DIR		=	builtins
EXEC_DIR		=	execution
REDIR_DIR		=	redirection
ENVP_DIR		=	envp
SIGN_DIR		=	signals
READING_DIR		=	reading
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
CFLAGS			=	-Wall -Wextra -Werror
LDFLAGS			=	-lreadline
RM				=	rm -f

# Source files
MAIN_FILES		=	main.c \
					main_utils.c \
					initialize_struct.c \
					initialization_utils.c \
					update_shlvl.c
BNF_FILES		=	input_validation.c \
					pipeline.c \
					command.c \
					simple_command.c \
					redirect.c \
					word.c
LEX_FILES		=	tokenization.c \
					token_creation.c \
					token_cleaning.c \
					extraction.c \
					skipping.c \
					merging.c \
					redirection.c \
					token_utils.c
PARS_FILES		=	cmd_creation.c \
					cmd_blocks.c \
					cmd_cleaning.c \
					cmd_utils.c
BUILT_FILES		=	cd.c \
					cd_utils.c \
					cd_utils2.c \
					env.c \
					export.c \
					export_utils.c \
					export_utils2.c \
					echo.c \
					exit.c \
					exit_utils.c \
					pwd.c \
					unset.c \
					key_handling.c \
					helper_functions.c
EXEC_FILES		=	builtin_check.c \
					one_child.c \
					multiple_children.c \
					find_path_and_execve.c \
					cmd_handling.c \
					pipe_and_redirect.c
REDIR_FILES		=	fds.c \
					heredoc.c \
					generate_filename.c
ENVP_FILES		=	check_all_tokens.c \
					handle_expansion.c \
					expansion_utils.c \
					expand_struct.c \
					spaces_in_expansion.c \
					tilde_expansion.c
READING_FILES	=	history.c \
					history_file.c
SIGN_FILES		=	handler.c
CLEAN_FILES		=	arr_cleaning.c \
					clean_struct.c \
					close_fds.c
ERR_FILES		=	errors.c \
					builtin_error.c \
					exit_error.c

SRC_FILES		=	$(addprefix $(MAIN_DIR)/, $(MAIN_FILES)) \
					$(addprefix $(BNF_DIR)/, $(BNF_FILES)) \
					$(addprefix $(LEX_DIR)/, $(LEX_FILES)) \
					$(addprefix $(PARS_DIR)/, $(PARS_FILES)) \
					$(addprefix $(EXEC_DIR)/, $(EXEC_FILES)) \
					$(addprefix $(REDIR_DIR)/, $(REDIR_FILES)) \
					$(addprefix $(ENVP_DIR)/, $(ENVP_FILES)) \
					$(addprefix $(READING_DIR)/, $(READING_FILES)) \
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
					@echo "$(GREEN)Created minishell!$(NC)"

# Object file compilation
$(OBJ_DIR)/%.o: 	$(SRC_DIR)/%.c
					@mkdir -p $(@D)
					@$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

# Cleaning rules
clean:
					@$(RM) -r $(OBJ_DIR)
					@make clean -C $(LIBFT_DIR)
					@echo "$(BLUE)*.o files removed$(NC)"
		
fclean:				clean 
					@$(RM) $(NAME)
					@$(RM) $(LIBFT)
					@$(RM) -r .build
					@echo "$(BLUE)All files removed$(NC)"
			
re:					fclean all

valgrind:
					valgrind --leak-check=full --show-reachable=yes --show-leak-kinds=all \
					--track-origins=yes --track-fds=yes --trace-children=yes \
					--suppressions=./val.supp \
					--gen-suppressions=all ./minishell

.PHONY:				all clean fclean re valgrind
