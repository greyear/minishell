#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
/*
int main() {
    char *input;

    // Display a prompt and read a line from the user
    input = readline("Enter something: ");

    if (input) {
        // Add the input to the history
        add_history(input);

        // Print the input back to the user
        printf("You entered: %s\n", input);

        // Free the dynamically allocated memory
        free(input);
    } else {
        printf("No input provided or EOF encountered.\n");
    }

    return 0;
}*/

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    char *input;

    printf("Press Ctrl+D to exit.\n");

    while (1) {
        input = readline("Enter something: "); // Prompt user input

        if (input == NULL) { // Handle EOF (Ctrl+D)
            printf("\nExiting...\n");
            break;
        }

        if (*input) { // Add non-empty input to the in-memory history
            add_history(input);
        }

        printf("You entered: %s\n", input);
        free(input); // Free memory allocated by readline
    }

    return 0;
}
