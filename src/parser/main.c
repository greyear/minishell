
#include "../../include/minishell.h"

#include <stdio.h>
#include <stdlib.h>

static void print_tokens(t_token *token_list)
{
	t_token *cur = token_list;

	printf("Tokens:\n");
	while (cur)
	{
		printf("Type: %d, Value: %s, Quotes: %c\n", cur->type, cur->data, cur->quote);
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
               cur->start ? cur->start->type : -1,  // Print type or -1 if NULL
               cur->end ? cur->end->data : "NULL",
               cur->end ? cur->end->type : -1);  // Print type or -1 if NULL
        cur = cur->next;
    }
}

int main(int argc, char **argv, char **envp)
{
    t_token *tokens;
    t_block *blocks;
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

    // Cleanup
    clean_token_list(&tokens);
    clean_block_list(&blocks);
    free(ms);

    return (0);
}
