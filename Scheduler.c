#include <stdlib.h>
#include <stdio.h>

#include "Scheduler.h"

Scheduler *initScheduler()
{
    Scheduler *scheduler = (Scheduler *)calloc(1, sizeof(Scheduler));

    if (scheduler == NULL)
    {
        printf("Failed to allocate memory for Scheduler.\n");
        return NULL;
    }

    BlockedQueue *bq = initBlockedQueue();
    ReadyQueue *rq = initReadyQueue();

    scheduler->blockedQueue = bq;
    scheduler->readyQueue = rq;

    return scheduler;
}

void addProcesses(Scheduler *scheduler, Process **arrayOfProcesses, int arrayCount)
{
    for (int i = 0; i < arrayCount; i++)
    {
        addReadyProcess(scheduler->readyQueue, arrayOfProcesses[i]);
    }
}

void timerInterrupt(Scheduler *scheduler)
{
    scheduler->currentMs++;
    if (isDone(scheduler)) {
        // Lidar com isso
    }
    unblockProcesses(scheduler);

    printf("rodei\n");

    if (scheduler->running != NULL) {
        scheduler->running->credits--;
        scheduler->running->currentBurst--;
        scheduler->running->leftCPUms--;
    }
    
    if (scheduler->running != NULL || scheduler->running->credits == 0 || scheduler->running->currentBurst == 0 || scheduler->running->leftCPUms == 0) {
        if (scheduler->running == NULL) { // Não existia running

        } else if (scheduler->running->leftCPUms == 0) {
            scheduler->exited = scheduler->running;
            scheduler->running = NULL;
        } else if (scheduler->running->currentBurst == NULL) { // Hora de fazer IO

        } else if (scheduler->running->credits == 0) { // Creditos acabaram, hora de trocar

        }
    }
}

void unblockProcesses(Scheduler *scheduler)
{
    Process *processUnblocked = dequeueBlockedProcess(scheduler->blockedQueue, scheduler->currentMs);
    while (processUnblocked != NULL)
    {
        addReadyProcess(scheduler->readyQueue, processUnblocked);
        processUnblocked = dequeueBlockedProcess(scheduler->blockedQueue, scheduler->currentMs);
    }
}

void recalculateCredits(Scheduler *scheduler)
{
    recalculateCreditsFromBlockeds(scheduler->blockedQueue);
    setNewCredits(scheduler->readyQueue);
    if (scheduler->running != NULL)
    {
        scheduler->running->credits >>= 2;
        scheduler->running->credits += scheduler->running->priority;
    }
}

int isDone(Scheduler *scheduler)
{
    return scheduler->readyQueue->size == 0 && scheduler->blockedQueue->size == 0 && scheduler->running == NULL;
}