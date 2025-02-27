#include "../../include/minishell.h"

char *generate_heredoc_filename(int index)
{
    char *filename;
    char buffer[20];
    int len = 0;
    int n = index; // Create base ".heredoc_"

    buffer[len++] = '.';
    buffer[len++] = 'h';
    buffer[len++] = 'e';
    buffer[len++] = 'r';
    buffer[len++] = 'e';
    buffer[len++] = 'd';
    buffer[len++] = 'o';
    buffer[len++] = 'c';
    buffer[len++] = '_'; // Convert index to string and append to buffer
    if (n == 0) {
        buffer[len++] = '0';
    } else {
        char digits[10];
        int digit_len = 0;
        while (n > 0) {
            digits[digit_len++] = '0' + (n % 10);
            n /= 10;
        }
        for (int i = digit_len - 1; i >= 0; i--) {
            buffer[len++] = digits[i];
        }
    }
    buffer[len] = '\0';

    // Allocate memory for the filename string
    filename = ft_strdup(buffer);
    if (!filename) {
        perror("heredoc: memory allocation failed");
        exit(1);
    }
    return filename;
}
