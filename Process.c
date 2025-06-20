#include <stdlib.h>

typedef struct Process
{
    char *name;

    unsigned int burstMs;
    unsigned int currentBurst;

    unsigned int ioMs;
    unsigned int currentIO;

    unsigned int totalCPUms;
    unsigned int leftCPUms;

    unsigned int order;
    unsigned int priority;

    unsigned int credits;

    unsigned int hasEntered;
    unsigned int enterTime;
} Process;

Process *initProcess(char *name,
                     unsigned int burstMS,
                     unsigned int ioMs,
                     unsigned int totalCPUms,
                     unsigned int order,
                     unsigned int priority)
{
    Process *newProcess = malloc(sizeof(Process));
    newProcess->name = name;

    newProcess->burstMs = burstMS;
    newProcess->currentBurst = burstMS;

    newProcess->ioMs = ioMs;
    newProcess->currentIO = ioMs;

    newProcess->totalCPUms = totalCPUms;
    newProcess->leftCPUms = totalCPUms;

    newProcess->order = order;
    newProcess->priority = priority;

    newProcess->credits = priority;

    newProcess->hasEntered = 0;
    newProcess->enterTime = 0;

    return newProcess;
}
