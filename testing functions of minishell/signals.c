#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h> // Для getpid() и pause()

// Обработчик сигнала
void signal_handler(int sig) {
    printf("Received signal: %d\n", sig);
}

int main() {
    struct sigaction sa; // Структура для настройки sigaction
    sigset_t signal_set; // Множество сигналов

    // Инициализируем множество сигналов как пустое
    if (sigemptyset(&signal_set) == -1) {
        perror("sigemptyset");
        exit(EXIT_FAILURE);
    }

    // Добавляем сигнал SIGUSR1 в множество
    if (sigaddset(&signal_set, SIGUSR1) == -1) {
        perror("sigaddset");
        exit(EXIT_FAILURE);
    }

    // Настройка обработчика сигнала
    sa.sa_handler = signal_handler; // Указываем функцию-обработчик
    sa.sa_mask = signal_set;        // Множество сигналов для блокировки
    sa.sa_flags = 0;                // Дополнительные флаги (0 — стандартное поведение)

    // Устанавливаем обработчик для SIGUSR1
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Process ID: %d\n", getpid());
    printf("Send SIGUSR1 to this process using 'kill -SIGUSR1 %d'\n", getpid());

    // Отправляем сигнал самому себе
    printf("Sending SIGUSR1 to self...\n");
    if (kill(getpid(), SIGUSR1) == -1) {
        perror("kill");
        exit(EXIT_FAILURE);
    }

    // Пауза для ожидания сигналов
    printf("Waiting for signals...\n");
    pause(); // Ждем сигналов

    return 0;
}
