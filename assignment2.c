#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *buf = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char buffer[11];
    ssize_t nr = 0;
    int fd;
    off_t size;

    fd = open("data.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if (fd < 0) {
        perror("Failed to open");
        return 1;
    }

    nr = write(fd, buf, 26);
    if (nr < 0) {
        perror("Failed to write");
        close(fd);
        return 1;
    }

    close(fd);

    fd = open("data.txt", O_RDWR);
    if (fd < 0) {
        perror("Failed to open");
        return 1;
    }

    size = lseek(fd, 0, SEEK_END);
    if (size < 0) {
        perror("Failed lseek");
        close(fd);
        return 1;
    }

    printf("Current file size: %ld bytes\n", (long)size);

    if (ftruncate(fd, 10) < 0) {
        perror("Failed ftruncate");
        close(fd);
        return 1;
    }

    size = lseek(fd, 0, SEEK_END);
    if (size == -1) {
        perror("Failed lseek");
        close(fd);
        return 1;
    }

    printf("Size after truncate: %ld bytes\n", (long)size);

    if (lseek(fd, 0, SEEK_SET) < 0) {
        perror("Failed lseek");
        close(fd);
        return 1;
    }

    nr = read(fd, buffer, 10);

    if (nr == -1) {
        perror("Failed to read");
        close(fd);
        return 1;
    }

    buffer[nr] = '\0';

    printf("Remaining content: %s\n", buffer); 

    close(fd);
    
    return 0;
}
