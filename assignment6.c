#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int fd;
    char number;
    char buffer[128];
    ssize_t ret;
    off_t position = 0;

    fd = open("numbers.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd < 0) {
        perror("Failed to open");
        return 1;
    }

    for (int i = 1; i <= 9; i++) {
        number = '0' + i;

        if (write(fd, &number, 1) < 0) {
            perror("Failed to write");
            close(fd);
            return 1;
        }

        if (write(fd, "\n", 1) < 0) {
            perror("Failed to write");
            close(fd);
            return 1;
        }
    }

    if (write(fd, "10\n", 3) < 0) {
        perror("Failed to write");
        close(fd);
        return 1;
    }

    close(fd);

    fd = open("numbers.txt", O_RDWR);

    if (fd < 0) {
        perror("Failed to open");
        return 1;
    }

    int count = 0;
    char ch;

    while (count < 3) {
        ret = read(fd, &ch, 1);

        if (ret < 0) {
            perror("Failed to read");
            close(fd);
            return 1;
        }

        if (ret == 0) {
            printf("File does not contain enough lines.\n");
            close(fd);
            return 1;
        }

        position++;

        if (ch == '\n') {
            count++;
        }
    }

    if (lseek(fd, position + 2, SEEK_SET) < 0) {
        perror("Failed lseek");
        close(fd);
        return 1;
    }

    ssize_t remainder_size = read(fd, buffer, sizeof(buffer));

    if (remainder_size < 0) {
        perror("Failed to read");
        close(fd);
        return 1;
    }

    if (lseek(fd, position, SEEK_SET) < 0) {
        perror("Failed lseek");
        close(fd);
        return 1;
    }

    if (write(fd, "100\n", 4) < 0) {
        perror("Failed to write");
        close(fd);
        return 1;
    }

    if (write(fd, buffer, remainder_size) < 0) {
        perror("Failed to write");
        close(fd);
        return 1;
    }

    if (lseek(fd, 0, SEEK_SET) < 0) {
        perror("Failed lseek");
        close(fd);
        return 1;
    }

    ret = read(fd, buffer, sizeof(buffer) - 1);

    if (ret < 0) {
        perror("Failed to read final content");
        close(fd);
        return 1;
    }

    buffer[ret] = '\0';

    printf("Final file content:\n%s", buffer);

    close(fd);

    return 0;
}

