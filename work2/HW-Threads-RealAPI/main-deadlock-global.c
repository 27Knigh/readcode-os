#include <stdio.h>

#include "mythreads.h"

pthread_mutex_t g = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

void* worker(void* arg) {
    Pthread_mutex_lock(&g);
    if ((long long) arg == 0) {
	Pthread_mutex_lock(&m1);
	Pthread_mutex_lock(&m2);
    } else {
	Pthread_mutex_lock(&m2);
	Pthread_mutex_lock(&m1);
    }
    Pthread_mutex_unlock(&m1);
    Pthread_mutex_unlock(&m2);
    Pthread_mutex_unlock(&g);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2;
    Pthread_create(&p1, NULL, worker, (void *) (long long) 0);
    Pthread_create(&p2, NULL, worker, (void *) (long long) 1);
    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    return 0;
}



//问题5：现在在 main-deadlock-global.c 上运行 helgrind。检查代码；它有与
//main-deadlock.c 相同的问题吗？ helgrind 应该报告同样的错误吗？对于像 helgrind 
//这样的工具，这告诉您什么？


//答：不存在和main-deadlock.c相同的问题。但是helgrind报告了相同的错误。helgrind报告有问题
//不一定有问题，而是可能存在问题。



