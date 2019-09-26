#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXCPU 1
/*
processor / core / thread
memory / L3,L2,L1 cache

int sched_setaffinity(pid_t pid, size_t cpusetsize, const cpu_set_t *mask)
int sched_getaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask)
int sched_getcpu(void);

cat /proc/cpuinfo
*/;

void print_available_cpu(cpu_set_t* mask)
{
    printf("MAX CPU # : %d\n", MAXCPU);
    for (int cpu=0; cpu<MAXCPU; cpu++) {
        printf("    %d cpu is %s available on this process.\n", cpu, CPU_ISSET(cpu, mask) ? "" : "not");
    }
}

int main()
{
    cpu_set_t mask;
    int cur_cpu;

    if (sched_getaffinity(getpid(), sizeof(cpu_set_t), &mask)) {
        perror("failed to sched_getaffinity");
        return -1;
    }
    print_available_cpu(&mask);

    cur_cpu = sched_getcpu();
    printf("process now working on %d cpu.\n", cur_cpu);

    CPU_ZERO(&mask);
    CPU_SET((cur_cpu+1)%MAXCPU, &mask);
    if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mask)) {
        perror("failed to sched_setaffinity");
        return -1;
    }
    
    cur_cpu = sched_getcpu();
    printf("process now working on %d cpu.\n", cur_cpu);
}