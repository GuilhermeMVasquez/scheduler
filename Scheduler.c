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
        scheduler->currentBiggestOrder++;
    }
}

void timerInterrupt(Scheduler *scheduler)
{
    unblockProcesses(scheduler);

    Process *running = scheduler->running;

    if (running != NULL)
    {
        if (!running->hasEntered)
        {
            running->hasEntered = 1;
            running->enterTime = scheduler->currentMs;
        }
        running->credits--;
        if (running->burstMs != 0)
            running->currentBurst--;
        running->leftCPUms--;
    }

    if (running == NULL || running->credits == 0 || (running->currentBurst == 0 && (running->burstMs != 0)) || running->leftCPUms == 0)
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
                if (isZeroCredits(scheduler->readyQueue))
                {
                    recalculateCredits(scheduler);
                }
                scheduler->running = dequeReadyProcess(scheduler->readyQueue);
            }
            else if ((running->burstMs != 0) && running->currentBurst == 0) // Hora de fazer IO
            {
                addBlockedProcess(scheduler->blockedQueue, running, scheduler->currentMs + running->ioMs + 1);
                scheduler->running = NULL;
                if (isZeroCredits(scheduler->readyQueue))
                {
                    recalculateCredits(scheduler);
                }
                scheduler->running = dequeReadyProcess(scheduler->readyQueue);
            }
            else if (running->credits == 0) // Creditos acabaram, hora de trocar
            {
                if (isZeroCredits(scheduler->readyQueue))
                {
                    recalculateCredits(scheduler);
                }
                scheduler->currentBiggestOrder++;
                scheduler->running->order = scheduler->currentBiggestOrder;
                scheduler->running = dequeReadyProcess(scheduler->readyQueue);
                addReadyProcess(scheduler->readyQueue, running);
            }
        }
        else
        {
            if (isZeroCredits(scheduler->readyQueue))
            {
                recalculateCredits(scheduler);
            }
            scheduler->running = dequeReadyProcess(scheduler->readyQueue);
        }
    }

    scheduler->currentMs++;

    printScheduler(scheduler);
}

void unblockProcesses(Scheduler *scheduler)
{
    Process *processUnblocked = dequeueBlockedProcess(scheduler->blockedQueue, scheduler->currentMs);
    while (processUnblocked != NULL)
    {
        scheduler->currentBiggestOrder++;
        processUnblocked->order = scheduler->currentBiggestOrder;
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

void printScheduler(Scheduler *scheduler)
{
    printf("\n");

    printf("Current time: %d\n", scheduler->currentMs);

    if (scheduler->running != NULL)
    {
        printf("Running:\n");
        printf("Process ID: %s, Credits: %d\n", scheduler->running->name, scheduler->running->credits);
    }

    printReadyQueue(scheduler->readyQueue);

    printBlockedQueue(scheduler->blockedQueue);

    if (scheduler->exited != NULL)
    {
        printf("Exited:\n");
        printf("Process ID: %s, Credits: %d, Turnaround time: %d\n", scheduler->exited->name, scheduler->exited->credits, scheduler->currentMs - scheduler->exited->enterTime);
        scheduler->exited = NULL;
    }
}
