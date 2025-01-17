#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main() {
    struct termios term;

    // Get current terminal attributes
    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        perror("tcgetattr");
        return 1;
    }

    // Disable echo
    term.c_lflag &= ~ECHO;
    //term.c_lflag &= ~ISIG; // Отключение сигналов например еще можно сделать
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1) {
        perror("tcsetattr");
        return 1;
    }

    printf("Echo disabled. Type something: ");
    char buf[100];
    fgets(buf, sizeof(buf), stdin);
    printf("\nYou typed: %s\n", buf);

    // Re-enable echo
    term.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    return 0;
}
