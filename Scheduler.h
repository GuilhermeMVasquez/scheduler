#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "SchedulerQueue.h"
#include "Process.h"

typedef struct Scheduler {
    SchedulerQueue *ready;    // Queue for ready processes
    Process *exited;          // Pointer to the last exited process
    Process *running;         // Pointer to the currently running process
} Scheduler;

#endif // SCHEDULER_H