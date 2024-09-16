#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include "BlockedQueue.h"

unsigned int on = 0;

int main()
{
    struct sigaction sa;
    struct itimerval timer;

    // sa.sa_handler = &PLACEHOLDER; passar o ponteiro da função que será chamada
    sa.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &sa, NULL);

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 1000;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 1000;

    setitimer(ITIMER_REAL, &timer, NULL);

    on = 1;

    int i = 0;

    while (on)
    {
        i += 1;
        printf("%d\n", i);
    }

    // just to test de blockedQueue
    // BlockedQueue *queue = initBlockedQueue();

    // Process *process = initProcess("hello",12,1,1,1,1);
    // Process *process2 = initProcess("hello2",12,1,1,1,1);
    // addBlockedProcess(queue,process, 4);
    // addBlockedProcess(queue,process2, 6);

    // printf("%s\n", dequeBlockedProcess(queue, 6)->name);
    // printf("%s\n", dequeBlockedProcess(queue, 6));

    return 0;
}
