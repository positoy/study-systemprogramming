#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// sigqueue 로 주고 sigaction_func 으로 받음
// kill -USR1 `pidof adv_signal_recv.out`
// ./adv_signal_send.out `pidof adv_signal_recv.out` 123

#ifdef SEND
int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("usage : %s <pid> <payload>\n", argv[0]);
        return -1;
    }
    int pid = atoi(argv[1]);
    int payload = atoi(argv[2]);

    union sigval sigdata = {
        .sival_int = payload
        // .sival_ptr = (void*)&largenum
    };

    if (sigqueue(pid, SIGUSR1, sigdata)) {
        perror("failed to sigqueue");
        return -1;
    }

    return 0;
}

#else
void sighandler_func(int signum){
    puts("sighandler");

    printf("%s signal receieved.\n",
        signum == SIGALRM ? "SIGALRM" : "Unknown");
}

void sigaction_func(int signum, siginfo_t *siginfo, void *sigdata){
    puts("sigaction");

    printf("%s signal receieved.\n",
        signum == SIGALRM ? "SIGALRM" :
        signum == SIGTERM ? "SIGTERM" :
        signum == SIGUSR1 ? "SIGUSR1" : "Unknown");

    if (siginfo->si_code == SI_QUEUE) {
        printf("signal from sigqueue(%d)\n", siginfo->si_int);
    } else {
        printf("signal from kill\n");
    }
}

int main(int argc, char** argv)
{
    // signal(SIGALRM, sighandler_func);

    struct sigaction sigact = {
        .sa_sigaction = sigaction_func,
        .sa_flags = SA_SIGINFO,
    };

    sigaction(SIGALRM, &sigact, NULL);
    sigaction(SIGTERM, &sigact, NULL);
    sigaction(SIGUSR1, &sigact, NULL);

    while(1) {
        usleep(100000);
    }
    return 0;
}
#endif
