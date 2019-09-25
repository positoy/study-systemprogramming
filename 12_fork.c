#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void test_func(const char* str) {
    printf("ppid(%d) -- pid(%d): %s\n", getppid(), getpid(), str);
}

int main(int argc, char **argv)
{
    pid_t pid, child_pid;
    int status;

    for (int i=0; i<10; i++) {
        pid = fork();
        if (pid == 0) {
            if (i == 2) {
                if (execl("/bin/ls", "ls", "-al", "/tmp", NULL) == -1) {
                    perror("failed to execl");
                    return -1;
                }
            }
            break;
        }
    }

    if (pid == -1) {
        perror("failed to fork");
        return -1;

    } else if (pid == 0) {
        test_func("child");
        return 0;

    } else if (pid > 0) {
        test_func("parent");

        while (1) {
            child_pid = wait(&status);
            if (child_pid == -1) {
                perror("failed to wait");
                return -1;
            } else {
                test_func(WIFEXITED(status) ? "WIFEXITED" : \
                    WEXITSTATUS(status) ? "WEXITSTATUS" : \
                    WIFSIGNALED(status) ? "WIFSIGNALED" : \
                    WTERMSIG(status) ? "WTERMSIG" : \
                    WCOREDUMP(status) ? "WCOREDUMP" : \
                    WIFSTOPPED(status) ? "WIFSTOPPED" : \
                    WSTOPSIG(status) ? "WSTOPSIG" : \
                    WIFCONTINUED(status) ? "WIFCONTINUED" : "Unknown");
            }
        }
    }
}
