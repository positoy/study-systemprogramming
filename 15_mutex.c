#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// sudo apt-get install glibc-doc, or
// sudo apt-get install manpages-posix-dev

// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_init
// pthread_mutex_lock
// pthread_mutex_trylock
// pthread_mutex_unlock
// pthread_mutex_destroy

pthread_t thread[2];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int shared = 0;
#define MAXTRY 100000000

void* thread_func(void* user_data)
{
    if (pthread_mutex_lock(&mutex)) {
        perror("failed to pthread_mutex_lock");
        exit(-1);
    }

    for (int i=0; i<MAXTRY; i++)
        shared++;
    for (int i=0; i<MAXTRY; i++)
        shared--;

    if (pthread_mutex_unlock(&mutex)) {
        perror("failed to pthread_mutex_unlock");
        exit(-1);
    }

    return NULL;
}

int main()
{
    for (unsigned long long i=0; i<2; i++) {
        if (pthread_create(&thread[i], NULL, thread_func, (void*)i)) {
            perror("failed to pthread_create");
            return -1;
        }
        printf("pthread_create : %lu\n", thread[i]);
    }

    for (unsigned long long i=0; i<2; i++) {
        if (pthread_join(thread[i], NULL)) {
            perror("failed to pthread_join");
            return -1;
        }
        printf("pthread_join : %lu\n", thread[i]);
    }

    printf( "shared : %d\n", shared);
}