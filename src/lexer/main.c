
#include "../../include/minishell.h"

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

int main(int argc, char **argv)
{
	t_token *tokens;

	if (argc != 2)
	{
		printf("Usage: %s \"command string\"\n", argv[0]);
		return (1);
	}

	tokens = tokenization(argv[1]);
	if (!tokens)
	{
		printf("Error: tokenization failed\n");
		return (1);
	}

	print_tokens(tokens);

	clean_token_list(&tokens);
	return (0);
}*/
