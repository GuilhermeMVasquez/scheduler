#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

#include "BlockedQueue.h"
#include "ReadyQueue.h"
#include "Scheduler.h"

Scheduler *scheduler;

void schedulerTimeInterruption(int signal)
{
    timerInterrupt(scheduler);
}

int main()
{
    struct sigaction sa;
    struct itimerval timer;

    scheduler = initScheduler();

    Process **arrayOfProcesses = malloc(4 * sizeof(Process *));
    arrayOfProcesses[0] = initProcess("A", 2, 5, 6, 1, 3);
    arrayOfProcesses[1] = initProcess("B", 3, 10, 6, 2, 3);
    arrayOfProcesses[2] = initProcess("C", 0, 0, 14, 3, 3);
    arrayOfProcesses[3] = initProcess("D", 0, 0, 10, 4, 3);

    addProcesses(scheduler, arrayOfProcesses, 4);

    sa.sa_handler = &schedulerTimeInterruption;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGALRM, &sa, NULL);

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 1000;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 1000;

    setitimer(ITIMER_REAL, &timer, NULL);

    while (!isDone(scheduler))
    {
    }

    printf("All processes have finished.\n");

    return 0;
}
