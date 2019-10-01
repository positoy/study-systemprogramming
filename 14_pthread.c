#include <pthread.h>
#include <error.h>
#include <stdio.h>
#include <unistd.h>

/*
pthread_create
pthread_exit
pthread_join
pthread_detach
*/

pthread_t threads[3];

void* thread_func(void* user_data)
{
    unsigned long long tid = (unsigned long long)user_data;

    if (tid == 2) {
        if (pthread_detach(threads[tid])) {
            perror("failed to pthread_detach");
            return (void*)-1;
        }
    }

    int until = tid == 0 ? 10 : tid == 1 ? 20 : tid == 2 ? 100 : 0;

    for (int i=0; i<until; i++) {
        printf("thread %llu counts %d\n", tid, i);
        usleep(300000);
    }

    void* retval = (void*)(tid * 10);
    pthread_exit(retval);

    return 0;
}

int main()
{

    for (unsigned long long i=0; i<3; i++) {
        if (pthread_create(&threads[i], NULL, thread_func, (void*)i))
            perror("failed to pthread_create");
    }

    void* retval;
    for (int i=0; i<3; i++) {
        pthread_join(threads[i], &retval);
        printf("thread %d returned %llu\n", i, (unsigned long long )retval);
    }

    sleep(30);
}