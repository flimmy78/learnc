#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
void *thr_fn1(void *arg)
{
    printf("thread 1 returning\n");
    sleep(3);
    return (void *) 1000;
}

void *thr_fn2(void *arg)
{
    printf("thread 2 exiting\n");
    sleep(3);
    pthread_exit((void *) 2000);
}

void *thr_fn3(void *arg)
{
    while (1) {
	printf("thread 3 writing, threadId: %u\n", pthread_self());
	sleep(1);
    }
}

int main(void)
{
    pthread_t tid;
    void *tret;
    pthread_create(&tid, NULL, thr_fn1, NULL);
    pthread_join(tid, &tret);
    printf("thread 1 exit code %d\n", (int) tret);
    pthread_create(&tid, NULL, thr_fn2, NULL);
    pthread_join(tid, &tret);
    printf("thread 2 exit code %d\n", (int) tret);
    pthread_create(&tid, NULL, thr_fn3, NULL);
    printf("main thread: %u sleeping\n", pthread_self());
    sleep(3);
    printf("main thread: %u wakeup\n", pthread_self());
    pthread_cancel(tid);
    pthread_join(tid, &tret);
    printf("thread 3 exit code %d\n", (int) tret);
    return 0;
}
