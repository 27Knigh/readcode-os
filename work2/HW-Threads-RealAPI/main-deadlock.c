#include <stdio.h>

#include "mythreads.h"

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

void* worker(void* arg) {
    if ((long long) arg == 0) {
	Pthread_mutex_lock(&m1);
	Pthread_mutex_lock(&m2);
    } else {
	Pthread_mutex_lock(&m2);
	Pthread_mutex_lock(&m1);
    }
    Pthread_mutex_unlock(&m1);
    Pthread_mutex_unlock(&m2);
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

//问题3：现在让我们看看 main-deadlock.c。检查代码。这段代码存在一个称为死锁的问题
//（我们将在下一章中更深入地讨论该问题）。你能看出它可能有什么问题吗？

//答：线程p1和线程p2可能会发生死锁，因为线程p1是先拿锁m1再拿锁m2，而线程p2则是先拿
//锁m2再拿锁m1，那么就有可能p1在刚拿到m1的时候，就发生了中断，然后p2执行把m2拿走了，
//这就可能会导致p1在等m2，而p2在等m1

//问题4：现在在此代码上运行 helgrind。 helgrind 报告什么？

//答：检测到出现获取锁的顺序不一致的错误
