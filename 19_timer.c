#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void sig_alarm_handler(int signum) {
    static int count = 0;
    
    if (signum == SIGALRM) puts("alarm!");
    else puts("another signal!");

    if (++count == 7) {
        struct itimerval tv;
        
        if (getitimer(ITIMER_REAL, &tv)) {
            perror("failed to getitimer");
            exit(-1);
        }
        printf("current timer : value(%lu, %lu) interval(%lu, %lu)\n", tv.it_value.tv_sec, tv.it_value.tv_usec, tv.it_interval.tv_sec, tv.it_interval.tv_usec);

        memset(&tv, 0, sizeof(struct itimerval));
        tv.it_value.tv_sec = 0;
        tv.it_value.tv_usec = 0;
        if (setitimer(ITIMER_REAL, &tv, NULL)) {
            perror("failed to setitimer");
            exit(-1);
        }
        puts("timer disabled");
    }
}

int main()
{
    // set timer callback
    if (signal(SIGALRM, sig_alarm_handler) == SIG_ERR) {
        perror("failed to signal");
        return -1;
    }

    // set one time timer
    alarm(1);
    sleep(2);
    
    // set interval timer
    struct itimerval tv = {
        .it_value = {1,500000},
        .it_interval = {0,500000}
    };
    if (setitimer(ITIMER_REAL, &tv, NULL)) {
        perror("failed to setitimer");
        return -1;
    }

    while(1) {
        usleep(500000);
    }
}