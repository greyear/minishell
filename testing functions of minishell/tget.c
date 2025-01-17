#include <stdio.h>
#include <stdlib.h>
#include <termcap.h>
#include <curses.h>
#include <unistd.h>

int main() {
    char term_buffer[2048];
    char *term_type = getenv("TERM");
    char *cursor_home, *cursor_move;
    char capability_buffer[64];
    int rows, cols;

    // Проверяем, задана ли переменная TERM
    if (!term_type) {
        fprintf(stderr, "The TERM environment variable is not set.\n");
        return 1;
    }

    // Инициализация termcap
    if (tgetent(term_buffer, term_type) <= 0) {
        fprintf(stderr, "Could not initialize termcap for terminal type: %s\n", term_type);
        return 1;
    }

    // Получаем количество строк и столбцов терминала
    rows = tgetnum("li"); // Количество строк (lines)
    cols = tgetnum("co"); // Количество столбцов (columns)
    printf("Terminal size: %d rows, %d columns\n", rows, cols);

    // Проверяем, поддерживает ли терминал перемещение курсора
    if (tgetflag("cm")) {
        printf("Terminal supports cursor movement.\n");
    } else {
        printf("Terminal does not support cursor movement.\n");
    }

    // Получаем строку для перемещения курсора в начало
    cursor_home = tgetstr("ho", NULL);
    if (cursor_home) {
        tputs(cursor_home, 1, putchar); // Перемещаем курсор в начало
    } else {
        printf("Failed to get home cursor capability.\n");
    }

    // Перемещаем курсор в произвольное место (например, на 5 строку и 10 столбец)
    cursor_move = tgetstr("cm", NULL);
    if (cursor_move) {
        char *move_to_5_10 = tgoto(cursor_move, 10, 5);
        tputs(move_to_5_10, 1, putchar); // Перемещаем курсор
        printf("Cursor moved to row 5, column 10\n");
    } else {
        printf("Failed to get cursor movement capability.\n");
    }

    return 0;
}
