#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

extern int errno;

int main() {
    int source_fd, destination_fd;
    char source[256];
    char destination[256];
    char buffer[1024];
    ssize_t ret;
    ssize_t total_bytes = 0;

    printf("Enter source file path: ");
    scanf("%255s", source);

    printf("Enter destination file path: ");
    scanf("%255s", destination);

    source_fd = open(source, O_RDONLY);
    if (source_fd == -1) {
        perror("Failed to open source\n");
        return 1;
    }

    destination_fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destination_fd == -1) {
        perror("Failed to open destination\n");
        close(source_fd);
        return 1;
    }

    while (1) {
        int len = sizeof(buffer);
        char *buf = buffer;

        while (len != 0 && (ret = read(source_fd, buf, len)) != 0) {
            if (ret == -1) {
                if (errno == EINTR) {
                    continue;
                }
                perror("Failed read\n");
                close(source_fd);
                close(destination_fd);
                return 1;
            }

            len -= ret;
            buf += ret;
        }

        int eof = (len != 0);
        int bytes_read = sizeof(buffer) - len;

        len = bytes_read;
        buf = buffer;

        while (len != 0 && (ret = write(destination_fd, buf, len)) != 0) {
            if (ret == -1) {
                if (errno == EINTR) {
                    continue;
                }
                perror("Failed write\n");
                close(source_fd);
                close(destination_fd);
                return 1;
            }

            len -= ret;
            buf += ret;
        }

        if (len != 0) {
            perror("Write returned 0\n");
            close(source_fd);
            close(destination_fd);
            return 1;
        }

        total_bytes += bytes_read;

        if (eof) {
            break;
        }
    }
       
    printf("File copied successfully!\n");
    printf("Total bytes copied: %zd\n", total_bytes);

    close(source_fd);
    close(destination_fd);
    return 0;
}

