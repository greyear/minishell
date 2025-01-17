#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

int main() {
    struct termios term;

    // Get terminal attributes
    if (ioctl(STDIN_FILENO, TCGETS, &term) == -1) {
        perror("ioctl");
        return 1;
    }

    // Modify attributes (e.g., disable canonical mode)
    term.c_lflag &= ~(ICANON);
    if (ioctl(STDIN_FILENO, TCSETS, &term) == -1) {
        perror("ioctl");
        return 1;
    }

    printf("Terminal set to raw mode. Press Ctrl+C to exit.\n");
    char c;
    while (read(STDIN_FILENO, &c, 1) > 0) {
        printf("You typed: %c\n", c);
    }

    return 0;
}


//????? 
