#include <stdio.h>

#include "mythreads.h"

// 
// simple synchronizer: allows one thread to wait for another
// structure "synchronizer_t" has all the needed data
// methods are:
//   init (called by one thread)
//   wait (to wait for a thread)
//   done (to indicate thread is done)
// 
typedef struct __synchronizer_t {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int done;
} synchronizer_t;

synchronizer_t s;

void signal_init(synchronizer_t *s) {
    Pthread_mutex_init(&s->lock, NULL);
    Pthread_cond_init(&s->cond, NULL);
    s->done = 0;
}

void signal_done(synchronizer_t *s) {
    Pthread_mutex_lock(&s->lock);
    s->done = 1;
    Pthread_cond_signal(&s->cond);
    Pthread_mutex_unlock(&s->lock);
}

void signal_wait(synchronizer_t *s) {
    Pthread_mutex_lock(&s->lock);
    while (s->done == 0)
	Pthread_cond_wait(&s->cond, &s->lock);
    Pthread_mutex_unlock(&s->lock);
}

void* worker(void* arg) {
    printf("this should print first\n");
    signal_done(&s);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p;
    signal_init(&s);
    Pthread_create(&p, NULL, worker, NULL);
    signal_wait(&s);
    printf("this should print last\n");

    return 0;
}


//问题8：现在看一下代码的稍微修改版本，该版本位于 main-signal-cv.c 中。
//此版本使用条件变量来执行信号发送（以及关联的锁定）。为什么此代码优于以
//前的版本？是正确性，还是性能，还是两者兼而有之？

//答：因为这里使用了条件变量来控制done的设置，这段代码，在子线程长时间运行且没有设置done时，
//父线程由于没有唤醒的信息，所以会被暂时挂起，这样就不会消耗计算机资源。

//问题9：再次在 main-signal-cv 上运行 helgrind。它报告任何错误吗？

//答：没有报告任何错误。