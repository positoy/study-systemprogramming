#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    char buff[300];
    int flag, ret;

    // blocking operation
    memset(buff, 0, sizeof(buff));
    if ((ret = read(STDIN_FILENO, buff, sizeof(buff))) == -1) goto error_read;
    puts(buff);

    // non-blocking operation (terminal session)
    if ((flag = fcntl(STDIN_FILENO, F_GETFL)) == -1) goto error_fcntl;
    if ((ret = fcntl(STDIN_FILENO, F_SETFL, flag | O_NONBLOCK)) == -1) goto error_fcntl;

    memset(buff, 0, sizeof(buff));
    if ((ret = read(STDIN_FILENO, buff, sizeof(buff))) == -1) goto error_read;
    puts(buff);

    return 0;

error_fcntl:
    perror("failed to fcntl");
    return -1;
error_read:
    perror("failed to read");
    return -1;
}