#include "../../include/minishell.h"

static void convert_index_to_digits(int index, char *buffer, int *len)
{
    char    digits[10];
    int     digit_len;
    int     i;
    
    digit_len = 0;
    if (index == 0)
    {
        buffer[*len] = '0';
        (*len)++;
        buffer[*len] = '\0';
        return;
    }
    while (index > 0)
    {
        digits[digit_len++] = '0' + (index % 10);
        index /= 10;
    }
    i = digit_len - 1;
    while (i >= 0)
    {
        buffer[(*len)++] = digits[i];
        i--;
    }
    buffer[(*len)] = '\0';
}

static void init_buffer(char *buffer, int *len)
{
    buffer[(*len)++] = '.';
    buffer[(*len)++] = 'h';
    buffer[(*len)++] = 'e';
    buffer[(*len)++] = 'r';
    buffer[(*len)++] = 'e';
    buffer[(*len)++] = 'd';
    buffer[(*len)++] = 'o';
    buffer[(*len)++] = 'c';
    buffer[(*len)++] = '_';
}

char    *generate_heredoc_filename(int index)
{
    char    *filename;
    char    buffer[20];
    int     len;
    
    len = 0;
    init_buffer(buffer, &len);
    convert_index_to_digits(index, buffer, &len);
    filename = ft_strdup(buffer);
    if (!filename)
    {
        perror("heredoc: memory allocation failed");
        exit(1);
    }
    return (filename);
}