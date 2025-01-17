#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    const char *filename = "example.txt";
    const char *symlinkname = "example_link";

    // Create a sample file
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to create file");
        return 1;
    }
    fprintf(file, "This is a sample file.\n");
    fclose(file);

    // Create a symbolic link
    if (symlink(filename, symlinkname) == -1) {
        perror("Failed to create symbolic link");
        return 1;
    }

    // Using stat (follows symbolic links)
    struct stat st;
    if (stat(symlinkname, &st) == 0) {
        printf("Using stat (symbolic link points to):\n");
        printf("File size: %ld bytes\n", st.st_size);
        printf("File permissions: %o\n", st.st_mode & 0777);
        printf("Last access time: %ld\n", st.st_atime);
    } else {
        perror("stat failed");
    }

    // Using lstat (does not follow symbolic links)
    struct stat lst;
    if (lstat(symlinkname, &lst) == 0) {
        printf("\nUsing lstat (symbolic link itself):\n");
        printf("Link size: %ld bytes\n", lst.st_size);
        printf("File permissions: %o\n", lst.st_mode & 0777);
        printf("Last access time: %ld\n", lst.st_atime);
    } else {
        perror("lstat failed");
    }

    // Using fstat (file descriptor)
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return 1;
    }

    struct stat fst;
    if (fstat(fd, &fst) == 0) {
        printf("\nUsing fstat:\n");
        printf("File size: %ld bytes\n", fst.st_size);
        printf("File permissions: %o\n", fst.st_mode & 0777);
        printf("Last access time: %ld\n", fst.st_atime);
    } else {
        perror("fstat failed");
    }

    // Clean up
    close(fd);
    unlink(symlinkname); // Remove the symbolic link
    remove(filename);    // Remove the file

    return 0;
}
