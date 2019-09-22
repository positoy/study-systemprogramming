#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/inotify.h>
#include <sys/epoll.h>

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
 
    if ((fd = inotify_init()) == -1)
        goto error_inotify;

    if ((ret = inotify_add_watch(fd, ".", IN_MODIFY | IN_CREATE | IN_DELETE)) == -1)
        goto error_inotify;

    createFile("./file_basic");
    if ((ret = inotify_add_watch(fd, "./file_basic", IN_MODIFY | IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_ACCESS)) == -1)
        goto error_inotify;
    
    int epfd;
    struct epoll_event event;
    
    if ((epfd = epoll_create(1)) == -1)
        goto error_epoll;

    memset(&event, 0, sizeof(struct epoll_event));
    event.events = EPOLLIN;
    event.data.fd = fd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event) == -1)
        goto error_epoll;

    memset(&event, 0, sizeof(struct epoll_event));
    event.events = EPOLLIN;
    event.data.fd = STDIN_FILENO;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event) == -1)
        goto error_epoll;

    while(1) {

        memset(&event, 0, sizeof(struct epoll_event));
        if ((ret = epoll_wait(epfd, &event, 1, 3000)) == -1)
            goto error;

        if (!ret)
        {
            puts("epoll timed out.");
        }
        else
        {
            printf("fd(%d) ", event.data.fd);

            memset(buf, 0, sizeof(buf));
            if (read(event.data.fd, buf, sizeof(buf)) == -1)
                goto error;

            if (event.data.fd == STDIN_FILENO)
            {
                printf("user input : %s", buf);
            }
            else if (event.data.fd == fd)
            {
                struct inotify_event* ievent = (struct inotify_event*)buf;

                if (ievent->mask & IN_MODIFY)
                    printf("wd(%d) : modify occured from file %s\n", ievent->wd, ievent->name);
                else if (ievent->mask & IN_CREATE)
                    printf("wd(%d) : create occured from file %s\n", ievent->wd, ievent->name);
                else if (ievent->mask & IN_DELETE)
                    printf("wd(%d) : delete occured from file %s\n", ievent->wd, ievent->name);
                else if (ievent->mask & IN_DELETE_SELF) {
                    printf("wd(%d) : delete self file\n", ievent->wd);
                    if (recoverInotify(fd, ievent, "./file_basic") == -1)
                        goto error_inotify;
                } else if (ievent->mask & IN_DELETE) {
                    printf("wd(%d) : access occured from file %s\n", ievent->wd, ievent->name);
                }
            }
        }
    }

    close(wd2);
    close(wd1);
    close(fd);
    close(epfd);
    return 0;

error_inotify:
    perror("failed to inotify sub routine");
    return -1;

error_epoll:
    perror("failed to epoll sub routine");
    return -1;

error:
    perror("faeild to read");
    close(wd2);
    close(wd1);
    close(fd);
    return -1;
}
