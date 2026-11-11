#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int fd;
    char filename[256];
    char c;
    off_t size;
    ssize_t ret;

    printf("Enter file path: ");
    scanf("%255s", filename);

    fd = open(filename, O_RDONLY);
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

    for (off_t position = size - 1; position >= 0; position--) {
        if (lseek(fd, position, SEEK_SET) < 0) {
            perror("Failed lseek");
            close(fd);
            return 1;
        }

        ret = read(fd, &c, 1);
        if (ret < 0) {
            perror("Failed to read");
            close(fd);
            return 1;
        }
        else if (ret == 1) {
            if (write(1, &c, 1) < 0) {
                perror("Failed to write");
                close(fd);
                return 1;
            }
        }
    }

    if (write(1, "\n", 1) < 0) {
        perror("Failed to write");
        close(fd);
        return 1;
    }

    close(fd);

    return 0;
}
