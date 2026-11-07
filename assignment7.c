#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int fd1, fd2;
    char file1[256], file2[256];
    char buffer1[256], buffer2[256];
    ssize_t n1, n2;
    int i;
    off_t byte_index = 0;

    printf("Enter first file path: ");
    scanf("%255s", file1);

    printf("Enter second file path: ");
    scanf("%255s", file2);

    // Open both files for reading
    fd1 = open(file1, O_RDONLY);
    if (fd1 < 0) {
        perror("Failed to open first file");
        return 1;
    }

    fd2 = open(file2, O_RDONLY);
    if (fd2 < 0) {
        perror("Failed to open second file");
        close(fd1);
        return 1;
    }

    while (1) {
        n1 = read(fd1, buffer1, sizeof(buffer1));
        n2 = read(fd2, buffer2, sizeof(buffer2));

        if (n1 < 0 || n2 < 0) {
            perror("Failed to read");
            close(fd1);
            close(fd2);
            return 1;
        }

        for (i = 0; i < n1 && i < n2; i++) {
            if (buffer1[i] != buffer2[i]) {
                printf("Files differ at byte %ld\n", (long)(byte_index + i));
                close(fd1);
                close(fd2);
                return 1;
            }
        }

        if (n1 != n2) {
            printf("Files differ at byte %ld\n", (long)(byte_index + i));
            close(fd1);
            close(fd2);
            return 1;
        }

        if (n1 == 0) {
            printf("Files are identical\n");
            break;
        }

        byte_index += n1;
    }

    close(fd1);
    close(fd2);

    return 0;
}
