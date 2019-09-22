#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

#if 0
typedef union epoll_data {
    void        *ptr;
    int          fd;
    uint32_t     u32;
    uint64_t     u64;
} epoll_data_t;

struct epoll_event {
    uint32_t     events;      /* Epoll events */
    epoll_data_t data;        /* User data variable */
};

int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
int epoll_pwait(int epfd, struct epoll_event *events, int maxevents, int timeout, const sigset_t *sigmask);
#endif

int main()
{
    int epfd;
    int fd;
    int ret;
    struct epoll_event event;

    epfd = epoll_create(1);
    if (epfd == -1) {
        perror("failed to epoll_create");
        return -1;
    }

    event.events = EPOLLIN;
    event.data.fd = STDIN_FILENO;
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);

    char buf[300];
    while (1) {
        ret = epoll_wait(epfd, &event, 1, 3000);
        memset(buf, 0, sizeof(buf));

        if (ret == -1) {
            perror("failed to epoll_wait");
            return -1;
        } else if (ret == 0) {
            puts("epoll_wait timed out.");
        } else {
            if (read(event.data.fd, buf, sizeof(buf)) == -1) {
                perror("failed to read");
                return -1;
            }
            printf("fd(%d) user inputs : %s", event.data.fd, buf);
        }
    }

    close(epfd);
    return 0;
}