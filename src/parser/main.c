
#include "../../include/minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

static void print_tokens(t_token *token_list)
{
	t_token *cur = token_list;

	printf("Tokens:\n");
	while (cur)
	{
		printf("Type: %d, Data: %s, Quotes: %c, Redir: %d, Ambig: %d, File: %s\n", cur->type, cur->data, cur->quote, cur->specific_redir, cur->ambiguous, cur->file);
		cur = cur->next;
	}
}

static void print_blocks(t_block *block_list)
{
	t_block *cur = block_list;

	printf("\nBlocks:\n");
	while (cur)
	{
		printf("Block Start: %s (Type: %d) | Block End: %s (Type: %d)\n",
		cur->start ? cur->start->data : "NULL",
		cur->start ? cur->start->type : (t_token_type)-1,  // <-- приведение типа
		cur->end ? cur->end->data : "NULL",
		cur->end ? cur->end->type : (t_token_type)-1);  // <-- приведение типа

		cur = cur->next;
	}
}

static void print_cmds(t_cmd *cmd_list)
{
	t_cmd *cur = cmd_list;

	printf("\nCommands:\n");
	while (cur)
	{
		printf("Command %d:\n", cur->num);
		printf("  Name: %s\n", cur->name ? cur->name : "NULL");

		if (cur->args)
		{
			printf("  Args: ");
			for (int i = 0; cur->args[i]; i++)
				printf("\"%s\" ", cur->args[i]);
			printf("\n");
		}
		else
			printf("  Args: NULL\n");

		cur = cur->next;
	}
}
/*
int main(int argc, char **argv, char **envp)
{
	t_token *tokens;
	t_block *blocks;
	t_cmd *cmds;
	t_ms *ms;
	int err_flag = 0;

	if (argc != 2)
	{
		printf("Usage: %s \"command string\"\n", argv[0]);
		return (1);
	}

	// Initialize shell struct
	ms = initialize_struct(envp);
	if (!ms)
	{
		printf("Error: failed to initialize shell structure\n");
		return (1);
	}

	// Tokenize input
	tokens = tokenization(argv[1], ms);
	if (!tokens)
	{
		printf("Error: tokenization failed\n");
		free(ms);
		return (1);
	}

	// Print tokens
	print_tokens(tokens);

	// Create blocks from tokens
	blocks = create_blocks_list(tokens, NULL, &err_flag);
	if (err_flag)
	{
		printf("Error: failed to create blocks\n");
		clean_token_list(&tokens);
		free(ms);
		return (1);
	}

	// Print blocks
	print_blocks(blocks);

	// Create commands from blocks
	cmds = create_cmd_list(blocks, ms);
	if (!cmds)
	{
		printf("Error: failed to create commands\n");
		clean_token_list(&tokens);
		clean_block_list(&blocks);
		free(ms);
		return (1);
	}

	// Print commands
	print_cmds(cmds);

	return (0);
}
*/

int main(int argc, char **argv, char **envp)
{
	t_ms *ms;
	t_token *tokens;
	t_block *blocks;
	t_cmd *cmds;
	char *input;
	int err_flag;

	// Проверяем, что программа запущена без аргументов
	if (argc != 1 && argv)
	{
		printf("Usage: ./minishell\n");
		return (1);
	}

	// Инициализируем структуру t_ms
	ms = initialize_struct(envp);
	if (!ms)
	{
		printf("Error: failed to initialize shell structure\n");
		return (1);
	}

	// Главный цикл минишелла
	while (1)
	{
		// Считываем ввод пользователя
		input = readline("minishell> ");
		if (!input) // Проверка EOF (Ctrl+D)
		{
			printf("exit\n");
			break;
		}

		// Проверяем ввод на соответствие BNF
		err_flag = validate_input(input);
		printf("BNF validation result: %d\n", err_flag);
		if (err_flag)
		{
			printf("Error: invalid input format\n");
			free(input);
			continue;
		}

		// Добавляем ввод в историю (если не пустой)
		if (*input)
			add_history(input);

		// Парсинг строки
		tokens = tokenization(input, ms);
		free(input); // Освобождаем readline-буфер

		if (!tokens)
		{
			printf("Error: tokenization failed\n");
			continue;
		}
		//???????????
		put_files_for_redirections(tokens);
		// Вывод токенов
		print_tokens(tokens);

		// Разбиваем токены на блоки
		blocks = create_blocks_list(tokens, NULL, &err_flag);
		if (err_flag)
		{
			printf("Error: failed to create blocks\n");
			clean_token_list(&tokens);
			continue;
		}

		// Вывод блоков
		print_blocks(blocks);

		// Создаём команды из блоков
		cmds = create_cmd_list(blocks, ms);
		if (!cmds)
		{
			printf("Error: failed to create commands\n");
			clean_token_list(&tokens);
			clean_block_list(&blocks);
			continue;
		}

		// Вывод команд
		print_cmds(cmds);

		/*// Очистка перед следующим вводом
		clean_token_list(&tokens);
		clean_block_list(&blocks);
		clean_cmd_list(&cmds);*/
	}

	// Освобождаем глобальные ресурсы
	free(ms);
	return (0);
}
