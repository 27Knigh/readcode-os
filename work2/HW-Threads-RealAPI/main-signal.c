#include <stdio.h>

#include "mythreads.h"

int done = 0;

void* worker(void* arg) {
    printf("this should print first\n");
    done = 1;
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p;
    Pthread_create(&p, NULL, worker, NULL);
    while (done == 0)
	;
    printf("this should print last\n");
    return 0;
}


//问题6：接下来我们看一下 main-signal.c。此代码使用变量 (done) 来表示子级已完成并且父级现在可以继续。
//为什么这段代码效率低下？ （父线程最终花时间做什么，特别是如果子线程需要很长时间才能完成？）


//答：这段代码效率低，是因为父线程必须等在子线程将done置为1后才会继续往后执行。特别是如果子线程
//需要很长时间才能完成时，父线程则一直在花时间在做无意义的循环，从而消耗cpu资源


//问题7：现在在此程序上运行 helgrind。它报告什么？代码正确吗？

//答：报告出现data race，但是代码时正确的。