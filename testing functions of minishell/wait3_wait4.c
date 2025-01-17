#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

void print_rusage(struct rusage *usage) {
    printf("User CPU time: %ld.%06lds\n", usage->ru_utime.tv_sec, usage->ru_utime.tv_usec);
    printf("System CPU time: %ld.%06lds\n", usage->ru_stime.tv_sec, usage->ru_stime.tv_usec);
    printf("Maximum resident set size: %ld KB\n", usage->ru_maxrss);
}

int main() {
    pid_t pid;
    struct rusage usage;
    int status;

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process: Simulate some work
        printf("Child process [%d]: Doing some work...\n", getpid());
        for (volatile int i = 0; i < 100000000; i++); // Busy loop to simulate CPU usage
        exit(42); // Exit with a specific status
    } else {
        // Parent process: Wait for the child using wait3
        pid_t child_pid = wait3(&status, 0, &usage);
        if (child_pid == -1) {
            perror("wait3");
            exit(EXIT_FAILURE);
        }

        printf("Child process [%d] exited with status %d\n", child_pid, WEXITSTATUS(status));
        printf("Resource usage for child process (wait3):\n");
        print_rusage(&usage);

        // Fork another child for wait4 example
        pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Another child process
            printf("Another child process [%d]: Doing some work...\n", getpid());
            for (volatile int i = 0; i < 200000000; i++); // More CPU work
            exit(84); // Exit with a different status
        } else {
            // Parent process: Wait for the specific child using wait4
            child_pid = wait4(pid, &status, 0, &usage);
            if (child_pid == -1) {
                perror("wait4");
                exit(EXIT_FAILURE);
            }

            printf("Another child process [%d] exited with status %d\n", child_pid, WEXITSTATUS(status));
            printf("Resource usage for another child process (wait4):\n");
            print_rusage(&usage);
        }
    }

    return 0;
}
