#include <stdio.h>

#include "mythreads.h"

int balance = 0;
pthread_mutex_t lock;

void* worker(void* arg) {
    Pthread_mutex_lock(&lock);
    balance++; // unprotected access 
    Pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p;

    Pthread_mutex_init(&lock, NULL);
    Pthread_create(&p, NULL, worker, NULL);

    Pthread_mutex_lock(&lock);
    balance++; // unprotected access
    Pthread_mutex_unlock(&lock);
    Pthread_join(p, NULL);
    return 0;
}


//问题1：首先构建 main-race.c。检查代码，以便您可以看到代码中的（希望是明显的）数据争用。
//现在运行 helgrind（通过输入 valgrind --tool=helgrind main-race）来查看它如何报告比赛。
//它是否指向正确的代码行？它还为您提供了哪些其他信息？

//答：使用helgrind工具检测后，可以正确的给出出现date race的代码行，同时也给出了线程的创建信息

//问题2：当您删除其中一行有问题的代码时会发生什么？现在，在共享变量的其中一个更新周围添加一个锁，
//然后在两个更新周围添加一个锁。 helgrind 在每个案例中报告了什么？

//答：删除其中任意一行错误代码后，helgrind检测显示程序0错误。只在共享变量balance中一处添加锁，
//仍然会出现data race。但是在balance出现的两处都加上锁则不会出现data race



