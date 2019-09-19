#include <stdio.h>
#include <string.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>

// flock do not work with "threads"

int main(int argc, char **argv)
{
    int fd, ops;

    if (argc < 2) {
        printf("usage : %s {flock option}\n", __FILE__);
        return -1;
    }

    if (!strcmp(argv[1], "sh"))
        ops = LOCK_SH;
    else if (!strcmp(argv[1], "ex"))
        ops = LOCK_EX;
    else if (!strcmp(argv[1], "nb"))
        ops = LOCK_NB;
    else
        ops = -1;

    fd = open("lockfile", O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        printf("failed to open");
        return -1;
    }

    puts("try to grab a lock");
    if (flock(fd, ops) == -1) goto error;
    puts("grabbed the lock");

    getc(stdin);

    puts("try to unlock");
    if (flock(fd, LOCK_UN) == -1) goto error;
    puts("unlock successful");
    return 0;

error:
    perror("failed to flock");
    close(fd);
    return -1;
}