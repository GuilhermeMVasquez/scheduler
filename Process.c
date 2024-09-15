#include <stdlib.h>

typedef struct Process {
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
} Process;

/*
Instancia um processo já malocado
*/
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

    newProcess->currentIO = ioMs;
    newProcess->currentIO = ioMs;

    newProcess->totalCPUms = totalCPUms;
    newProcess->leftCPUms = totalCPUms;

    newProcess->order = order;
    newProcess->priority = priority;

    newProcess->credits = priority;

    return newProcess;
}
