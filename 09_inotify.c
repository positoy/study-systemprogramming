#include <stdio.h>
#include <sys/inotify.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int fd, wd1, wd2;
    int ret = -1;
    char buf[300];
    struct inotify_event *event = NULL;

    fd = inotify_init();
    if (fd == -1) {
        perror("failed to inotify_init");
        return -1;
    }

    ret = inotify_add_watch(fd, ".", IN_MODIFY | IN_CREATE | IN_DELETE);
    if (ret == -1) {
        perror("failed to inotify_add_event");
        return -1;
    }

    ret = inotify_add_watch(fd, "./file_basic", IN_MODIFY | IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_ACCESS);
    if (ret == -1) {
        perror("failed to inotify_add_event");
        return -1;
    }

    while (1) {
        if (read(fd, buf, sizeof(buf)) == -1) {
            perror("failed to read");
            return -1;
        }

        event = (struct inotify_event*)buf;
        if (event->mask & IN_MODIFY)
            printf("wd(%d) : modify occured from file %s\n", event->wd, event->name);
        else if (event->mask & IN_CREATE)
            printf("wd(%d) : create occured from file %s\n", event->wd, event->name);
        else if (event->mask & IN_DELETE)
            printf("wd(%d) : delete occured from file %s\n", event->wd, event->name);
        else if (event->mask & IN_DELETE_SELF)
            printf("wd(%d) : delete self occured from file %s\n", event->wd, event->name);
        else if (event->mask & IN_DELETE)
            printf("wd(%d) : access occured from file %s\n", event->wd, event->name);
    }

    close(wd2);
    close(wd1);
    close(fd);

    return 0;
}

#if 0

int inotify_init(void);
int inotify_add_watch(int fd, const char *pathname, uint32_t mask);
int inotify_rm_watch(int fd, int wd);

struct inotify_event {
    int      wd;       /* Watch descriptor */
    uint32_t mask;     /* Mask describing event */
    uint32_t cookie;   /* Unique cookie associating related
                            events (for rename(2)) */
    uint32_t len;      /* Size of name field */
    char     name[];   /* Optional null-terminated name */
};

/* Supported events suitable for MASK parameter of INOTIFY_ADD_WATCH.  */
#define IN_ACCESS	 0x00000001	/* File was accessed.  */
#define IN_MODIFY	 0x00000002	/* File was modified.  */
#define IN_ATTRIB	 0x00000004	/* Metadata changed.  */
#define IN_CLOSE_WRITE	 0x00000008	/* Writtable file was closed.  */
#define IN_CLOSE_NOWRITE 0x00000010	/* Unwrittable file closed.  */
#define IN_CLOSE	 (IN_CLOSE_WRITE | IN_CLOSE_NOWRITE) /* Close.  */
#define IN_OPEN		 0x00000020	/* File was opened.  */
#define IN_MOVED_FROM	 0x00000040	/* File was moved from X.  */
#define IN_MOVED_TO      0x00000080	/* File was moved to Y.  */
#define IN_MOVE		 (IN_MOVED_FROM | IN_MOVED_TO) /* Moves.  */
#define IN_CREATE	 0x00000100	/* Subfile was created.  */
#define IN_DELETE	 0x00000200	/* Subfile was deleted.  */
#define IN_DELETE_SELF	 0x00000400	/* Self was deleted.  */
#define IN_MOVE_SELF	 0x00000800	/* Self was moved.  */

#endif
