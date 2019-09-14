#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>

struct person {
    char name[10];
    int age;
};

const char filepath[] = "./people.db";

int write_info(struct person* p)
{
    int fd;
    fd = open(filepath, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("failed to open");
        return -1;
    }

    if (write(fd, p, sizeof(struct person)) == -1) {
        perror("failed to write");
        if (close(fd))
            perror("failed to close");
        return -1;
    }

    if (close(fd)) {
        perror("failed to close");
        return -1;
    }
}

int dump_info()
{
    int fd;
    struct person p;

    fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("failed to open");
        return -1;
    }

    ssize_t readLength = 0;
    while (readLength = read(fd, &p, sizeof(struct person))) {
        if (readLength == -1) {
            perror("failed to read");
            if (close(fd))
                perror("failed to close");
            return -1;
        }
        printf("name : %s, age : %d\n", p.name, p.age);
    };

    if (readLength == 0) {
        FILE* fptr = fdopen(fd, "r");
        if (fptr) {
            int position = ftell(fptr);
            if (position == -1) {
                perror("failed to ftell");
                close(fd);
                return -1;
            }
            printf("file length : %d\n", position);
        } else {
            perror("failed to fdopen");
        }
        puts("successfully read until eof");
        close(fd);
    }
    
    return 0;
}

// open
// read, pread
// write, pwrite
// close

int main()
{
    struct person people[] = {
        { "chang", 32 },
        { "sim", 30 }
    };

    write_info(&people[0]);
    write_info(&people[1]);
    dump_info();
}
