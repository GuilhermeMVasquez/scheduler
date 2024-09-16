#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "ReadyQueue.h"
#include "Process.h"

typedef struct Scheduler {
    ReadyQueue *ready;    // Queue for ready processes
    
    Process *exited;          // Pointer to the last exited process
    Process *running;         // Pointer to the currently running process
} Scheduler;

#endif // SCHEDULER_H