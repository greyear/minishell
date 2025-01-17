#include <stdio.h>
#include <stdlib.h>

int main() {
    char *path = getenv("PATH");
    if (path) {
        printf("PATH: %s\n", path);
    } else {
        printf("PATH not found in environment variables.\n");
    }
    return 0;
}
