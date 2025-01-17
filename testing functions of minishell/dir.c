#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char cwd[1024];
    
    // 1. Get the current working directory
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd failed");
        return 1;
    }
    printf("Current working directory: %s\n", cwd);

    // 2. Change the current working directory
    const char *new_dir = "/tmp";
    if (chdir(new_dir) == -1) {
        perror("chdir failed");
        return 1;
    }
    printf("Changed working directory to: %s\n", new_dir);

    // 3. Get the updated current working directory
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd failed");
        return 1;
    }
    printf("Updated working directory: %s\n", cwd);

    // 4. Open the current directory
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir failed");
        return 1;
    }
    printf("Opened directory: %s\n", cwd);

    // 5. Read entries from the directory
    struct dirent *entry;
    printf("Directory contents:\n");
    while ((entry = readdir(dir)) != NULL) {
        printf("  %s\n", entry->d_name);
    }

    // 6. Close the directory
    if (closedir(dir) == -1) {
        perror("closedir failed");
        return 1;
    }
    printf("Directory closed successfully.\n");

    // 7. Change back to the original working directory
    if (chdir("/") == -1) { // Change to root for safety if returning to original is not needed
        perror("chdir back failed");
        return 1;
    }
    printf("Changed back to root directory.\n");

    return 0;
}
