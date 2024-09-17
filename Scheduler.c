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

    if (!isDone(scheduler))
    {
        unblockProcesses(scheduler);

        Process *running = scheduler->running;

        if (running != NULL)
        {
            running->credits--;
            running->currentBurst--;
            running->leftCPUms--;
        }

        if (running == NULL || running->credits == 0 || running->currentBurst == 0 || running->leftCPUms == 0)
        {
            if (running != NULL) // Existe running
            {
                if (running->leftCPUms == 0) // Processo terminou
                {
                    scheduler->exited = running;
                    scheduler->running = NULL;
                }
                else if (running->currentBurst == 0) // Hora de fazer IO
                {
                    addBlockedProcess(scheduler->blockedQueue, running, scheduler->currentMs + running->ioMs);
                    scheduler->running = NULL;
                }
                else if (running->credits == 0) // Creditos acabaram, hora de trocar
                {
                    addReadyProcess(scheduler->readyQueue, running);
                    scheduler->running = NULL;
                }
            }

            if (isZeroCredits(scheduler->readyQueue)) // Todos os processos tem 0 creditos
            {
                recalculateCredits(scheduler);
            }

            scheduler->running = dequeReadyProcess(scheduler->readyQueue);
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