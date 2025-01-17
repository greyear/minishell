#include <stdio.h>
#include <unistd.h>

int main() {
    if (isatty(STDIN_FILENO)) {
        printf("Standard input is a terminal: %s\n", ttyname(STDIN_FILENO));
        printf("Terminal slot number: %d\n", ttyslot());
    } else {
        printf("Standard input is not a terminal.\n");
    }
    return 0;
}
