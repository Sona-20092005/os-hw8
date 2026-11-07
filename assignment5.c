#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int fd;
    off_t size;

    fd = open("sparse.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Failed to open");
        return 1;
    }

    if (write(fd, "START", 5) < 0) {
        perror("Failed to write");
        close(fd);
        return 1;
    }

    if (lseek(fd, 1024 * 1024, SEEK_CUR) < 0) {
        perror("Failed to lseek");
        close(fd);
        return 1;
    }

    if (write(fd, "END", 3) < 0) {
        perror("Failed to write");
        close(fd);
        return 1;
    }

    close(fd);

    fd = open("sparse.bin", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open");
        return 1;
    }

    size = lseek(fd, 0, SEEK_END);
    if (size < 0) {
        perror("Failed to lseek");
        close(fd);
        return 1;
    }

    printf("Apparent file size: %ld bytes\n", (long)size);

    close(fd);

    return 0;
}
