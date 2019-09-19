#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

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
    struct stat st;
    struct person *pptr;

    fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("failed to open");
        return -1;
    }

    if (fstat(fd, &st) == -1) {
        perror("failed to fstat");
        close(fd);
        return -1;
    }

/*
     void *
     mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
*/
    if ((pptr = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        perror("failed to mmap");
        close(fd);
        return -1;
    }

    for (int i=0; i<st.st_size/sizeof(struct person); i++)
        printf("name : %s, age : %d\n", pptr[i].name, pptr[i].age);

    close(fd);
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
    puts("hello");
    dump_info();
}