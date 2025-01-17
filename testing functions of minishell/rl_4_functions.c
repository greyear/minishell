#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    char *input;

    printf("Interactive readline demonstration. Type commands below.\n");
    printf("Commands:\n");
    printf("  clear   - Clears the history\n");
    printf("  replace - Replaces the current line with 'Replaced line'\n");
    printf("  exit    - Exits the program\n");

    while (1) {
        // Read user input
        input = readline("Enter something: ");

        if (input == NULL) { // Handle EOF (Ctrl+D)
            printf("\nExiting...\n");
            break;
        }

        if (*input) { // Add non-empty input to the history
            add_history(input);
        }

        // Command Handling
        if (strcmp(input, "clear") == 0) {
            rl_clear_history(); // Clear all history
            printf("History cleared.\n");
        } else if (strcmp(input, "replace") == 0) {
            rl_replace_line("Replaced line", 0); // Replace current input buffer
            rl_on_new_line();                    // Indicate a new line
            rl_redisplay();                      // Update display
        } else if (strcmp(input, "exit") == 0) {
            free(input);
            break; // Exit the loop
        } else {
            printf("You entered: %s\n", input);
        }

        free(input); // Free memory allocated by readline
    }

    return 0;
}
