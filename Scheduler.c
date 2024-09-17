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
    unblockProcesses(scheduler);

    Process *running = scheduler->running;

    if (running != NULL)
    {
        running->credits--;
        if (running->burstMs != 0)
            running->currentBurst--;
        running->leftCPUms--;
    }

    if (running == NULL || running->credits == 0 || (running->currentBurst == 0 && running->burstMs != 0) || running->leftCPUms == 0)
    {
        if (running != NULL) // Existe running
        {
            if (running->leftCPUms == 0) // Processo terminou
            {
                if (scheduler->exited != NULL)
                {
                    free(scheduler->exited);
                }
                scheduler->exited = running;
                scheduler->running = NULL;
                printf("Process %s exited at %dms. [Credits: %d, Current Burst: %d, Left CPU ms: %d]\n", running->name, scheduler->currentMs, running->credits, running->currentBurst, running->leftCPUms);
            }
            else if (running->currentBurst == 0) // Hora de fazer IO
            {
                addBlockedProcess(scheduler->blockedQueue, running, scheduler->currentMs + running->ioMs + 1);
                scheduler->running = NULL;
                printf("Process %s blocked at %dms. [Credits: %d, Current Burst: %d, Left CPU ms: %d]\n", running->name, scheduler->currentMs, running->credits, running->currentBurst, running->leftCPUms);
                if (isZeroCredits(scheduler->readyQueue))
                {
                    recalculateCredits(scheduler);
                }
                scheduler->running = dequeReadyProcess(scheduler->readyQueue);
                if (scheduler->running != NULL)
                {
                    printf("Process %s started at %dms. [Credits: %d, Current Burst: %d, Left CPU ms: %d]\n", scheduler->running->name, scheduler->currentMs, scheduler->running->credits, scheduler->running->currentBurst, scheduler->running->leftCPUms);
                }
            }
            else if (running->credits == 0) // Creditos acabaram, hora de trocar
            {
                if (isZeroCredits(scheduler->readyQueue))
                {
                    recalculateCredits(scheduler);
                }
                scheduler->running = dequeReadyProcess(scheduler->readyQueue);
                addReadyProcess(scheduler->readyQueue, running);
                printf("Process %s preempted at %dms. [Credits: %d, Current Burst: %d, Left CPU ms: %d]\n", running->name, scheduler->currentMs, running->credits, running->currentBurst, running->leftCPUms);
                if (scheduler->running != NULL)
                {
                    printf("Process %s started at %dms. [Credits: %d, Current Burst: %d, Left CPU ms: %d]\n", scheduler->running->name, scheduler->currentMs, scheduler->running->credits, scheduler->running->currentBurst, scheduler->running->leftCPUms);
                }
            }
        }
        else
        {
            scheduler->running = dequeReadyProcess(scheduler->readyQueue);
            if (scheduler->running != NULL)
            {
                printf("Process %s started at %dms. [Credits: %d, Current Burst: %d, Left CPU ms: %d]\n", scheduler->running->name, scheduler->currentMs, scheduler->running->credits, scheduler->running->currentBurst, scheduler->running->leftCPUms);
            }
        }
    }

    scheduler->currentMs++;
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
    scheduler->readyQueue = setNewCredits(scheduler->readyQueue);
    if (scheduler->running != NULL)
    {
        scheduler->running->credits >>= 1;
        scheduler->running->credits += scheduler->running->priority;
    }
}

int isDone(Scheduler *scheduler)
{
    return scheduler->readyQueue->size == 0 && scheduler->blockedQueue->size == 0 && scheduler->running == NULL;
}