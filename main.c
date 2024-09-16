#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include "BlockedQueue.h"
#include "ReadyQueue.h"

unsigned int on = 0;

void print_hey(int signum)
{
    printf("Hey! %d\n", signum);
}

int main()
{
    struct sigaction sa;
    struct itimerval timer;

    sa.sa_handler = &print_hey;
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
        if (i % 1000000 == 0)
        {
            printf("%d\n", i);
        }
    }

    // Testando qyeye

    // ReadyQueue *queue = initReadyQueue();
    // Process *process = initProcess("pedro", 12, 1, 1, 1, 1);
    // Process *process2 = initProcess("guilherme", 12, 1, 1, 2, 1);
    // Process *process3 = initProcess("luca", 12, 1, 2, 4, 1);
    // Process *process4 = initProcess("henrique", 12, 1, 1, 6, 3);
    // Process *process5 = initProcess("wertyu", 12, 1, 1, 5, 1);
    // Process *process6 = initProcess("asdf", 12, 1, 4, 5, 5);
    // addReadyProcess(queue, process);
    // addReadyProcess(queue, process2);
    // addReadyProcess(queue, process3);
    // addReadyProcess(queue, process4);
    // addReadyProcess(queue, process5);
    // addReadyProcess(queue, process6);

    // printReadyQueue(queue);

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
