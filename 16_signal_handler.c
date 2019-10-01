/*
#include <signal.h>

signal / disposition (Term, Ign, Core, Stop, Cont)

* SIG_IGN
* SIG_DFL
* sighandler (signal, sigaction)

# kill, killall -signum/-s sigterm <pid>
# pkill, pgrep, pidof <process name>

typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);

sigprocmask(how, set, oldset); how - SIG_BLOCK, SIG_UNBLOCK, SIG_SETMASK

sigemptyset(set)
sigfillset(set)
sigaddset(set, signum)
sigdelset(set, signum)
sigismember(set, signum)
*/

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

// kill 로 주고 signal 로 받음

void handler(int signum) {
    printf("gonna ignore this signal(%d)\n", signum);
}

int main()
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGTERM);
    sigaddset(&set, SIGKILL); // not ignorable
    sigprocmask(SIG_SETMASK, &set, NULL);

    signal(SIGTERM, handler);

    puts("start of while(1)");
    while (1) {
        sleep(1);
    }

    return 0;
}