#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/inotify.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
 * I/O MULTIPLEXNIG
 *
 * 1. polling with non-blocking mode
 * 2. multi-thread
 * 3. I/O MULTIPLEXNIG (select, epoll)
 *
 */

int createFile(const char* filepath)
{
    int fd;

    if (access(filepath, F_OK) == 0) {
        printf("file already exist. (%s)\n ", filepath);
        return 0;
    }

    if ((fd = open("./file_basic", O_WRONLY | O_CREAT, 0644)) == -1)
        return -1;

    if (close(fd) == -1)
        return -1;

    return 0;
}


int recoverInotify(int fd, struct inotify_event* event, const char* filepath)
{

    if (inotify_rm_watch(fd, event->wd) == -1)
        goto error;
    if (createFile(filepath) == -1)
        goto error;
    if (inotify_add_watch(fd, filepath, IN_MODIFY | IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_ACCESS) == -1)
        goto error;

    printf("recoverInotify : %s\n", filepath);
    return 0;
error:
    perror("recoverInotify");
    return -1;
}

int main()
{
    int fd, wd1, wd2;
    int ret = -1;
    char buf[300];
    fd_set fds;
    struct timeval timeout;
    struct inotify_event *event = NULL;

    fd = inotify_init();
    if (fd == -1) {
        perror("failed to inotify_init");
        return -1;
    }

    ret = inotify_add_watch(fd, ".", IN_MODIFY | IN_CREATE | IN_DELETE);
    if (ret == -1)
        goto error_inotify;

    createFile("./file_basic");
    ret = inotify_add_watch(fd, "./file_basic", IN_MODIFY | IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_ACCESS);
    if (ret == -1)
        goto error_inotify;

    while(1) {

        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        FD_SET(fd, &fds);

        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        ret = select(fd > STDIN_FILENO ? fd+1 : STDIN_FILENO+1, &fds, NULL, NULL, &timeout);

        if (ret == -1) {
            goto error;
        } else if (ret == 0) {
            puts("select timed out.");
        } else {
            memset(buf, 0, sizeof(buf));

            if (FD_ISSET(STDIN_FILENO, &fds)) {
                if (read(STDIN_FILENO, buf, sizeof(buf)) == -1)
                    goto error;
                
                printf("user input : %s", buf);

            } else if (FD_ISSET(fd, &fds)) {
                if (read(fd, buf, sizeof(buf)) == -1)
                    goto error;

                event = (struct inotify_event*)buf;

                if (event->mask & IN_MODIFY)
                    printf("wd(%d) : modify occured from file %s\n", event->wd, event->name);
                else if (event->mask & IN_CREATE)
                    printf("wd(%d) : create occured from file %s\n", event->wd, event->name);
                else if (event->mask & IN_DELETE)
                    printf("wd(%d) : delete occured from file %s\n", event->wd, event->name);
                else if (event->mask & IN_DELETE_SELF) {
                    printf("wd(%d) : delete self file\n", event->wd);
                    if (recoverInotify(fd, event, "./file_basic") == -1)
                        goto error_inotify;
                } else if (event->mask & IN_DELETE)
                    printf("wd(%d) : access occured from file %s\n", event->wd, event->name);
            }
        }
    }

    close(wd2);
    close(wd1);
    close(fd);
    return 0;

error_inotify:
    perror("failed to inotify_add_event");
    return -1;

error:
    perror("faeild to read");
    close(wd2);
    close(wd1);
    close(fd);
    return -1;
}
