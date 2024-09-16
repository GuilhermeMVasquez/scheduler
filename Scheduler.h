#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "ReadyQueue.h"
#include "BlockedQueue.h"
#include "Process.h"

typedef struct Scheduler
{
    ReadyQueue *readyQueue;     // Queue for ready processes
    BlockedQueue *blockedQueue; // Queue for blocked processes

    Process *exited;  // Pointer to the last exited process
    Process *running; // Pointer to the currently running process

    unsigned int currentMs;
} Scheduler;

Scheduler *initScheduler();

void addProcesses(Scheduler *scheduler, Process **arrayOfProcesses, int arrayCount);

void timerInterrupt(Scheduler *scheduler);

void unblockProcesses(Scheduler *scheduler);

void recalculateCredits(Scheduler *scheduler);

int isDone(Scheduler *scheduler);

#endif // SCHEDULER_H