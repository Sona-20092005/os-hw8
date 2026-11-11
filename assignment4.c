#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int fd;
    char input[256];
    char prefix[50];
    ssize_t nr_read;
    ssize_t nr_write;
    int len;
    off_t position;

    fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        perror("Failed to open");
        return 1;
    }

    printf("Enter message:\n");

    nr_read = read(0, input, sizeof(input));
    if (nr_read < 0) {
        perror("Failed to read");
        close(fd);
        return 1;
    }
    for (int i = 0; i < nr_read; i++) {
        if (input[i] == '\n') {
            nr_read = i;
            break;
        }
    }

    len = sprintf(prefix, "PID=%ld: ", (long)getpid());

    nr_write = write(fd, prefix, len);
    if (nr_write < 0) {
        perror("Failed to write");
        close(fd);
        return 1;
    }

    nr_write = write(fd, input, nr_read);
    if (nr_write < 0) {
        perror("Failed to write");
        close(fd);
        return 1;
    }

    nr_write = write(fd, "\n", 1);
    if (nr_write < 0) {
        perror("Failed to write");
        close(fd);
        return 1;
    }


    position = lseek(fd, 0, SEEK_CUR);
    if (position < 0) {
        perror("Failed lseek");
        close(fd);
        return 1;
    }

    printf("Final file offset: %ld\n", (long)position);

    close(fd);

    // O_APPEND makes sure that each write adds new data to the end of the file.
    // After writing to that file, the current pointer/offser advances to the new end of that file.
    // And so SEEK_CUR returns this current pointer/offset.

    return 0;
}
