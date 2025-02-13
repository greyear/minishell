
#include "../../include/minishell.h"

#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>

/*
void print_tokens(t_token *token_list)
{
	t_token *cur = token_list;

	printf("Tokens:\n");
	while (cur)
	{
		printf("Type: %d, Value: %s, Quotes: %c\n", cur->type, cur->data, cur->quote);
		cur = cur->next;
	}
}

int main(int argc, char **argv, char **envp)
{
	t_token *tokens;
	t_ms *ms;

	if (argc != 2)
	{
		printf("Usage: %s \"command string\"\n", argv[0]);
		return (1);
	}

	// Initialize ms struct
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
		free(ms); // Free allocated ms before exiting
		return (1);
	}

	// Print tokenized result
	print_tokens(tokens);

	// Cleanup
	clean_token_list(&tokens);
	free(ms);

	return (0);
}*/
