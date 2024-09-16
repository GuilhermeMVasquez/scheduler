#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "ReadyQueue.h"
#include "BlockedQueue.h"
#include "Process.h"

typedef struct Scheduler {
    ReadyQueue *readyQueue;    // Queue for ready processes
    BlockedQueue *blockedQueue; // 

    Process *exited;          // Pointer to the last exited process
    Process *running;         // Pointer to the currently running process
} Scheduler;

Scheduler *initScheduler();

void addProcesses(Scheduler *scheduler, Process **arrayOfProcesses, int arrayCount);

void timerInterrupt();

#endif // SCHEDULER_H