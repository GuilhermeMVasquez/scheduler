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
        arrayOfProcesses[i]->entryTime = scheduler->currentMs; // Register arrival time
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

                unsigned int turnaroundTime = scheduler ->currentMs - scheduler->exited->entryTime;
                printf("Process %s finished. Turnaround time: %d ms\n", scheduler->exited->name, turnaroundTime);

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

    // Exibe o tempo atual
    printf("Current time: %d ms\n", scheduler->currentMs);

    // Exibe o processo em execução
    if (scheduler->running != NULL)
    {
        printf("Running Process: %s, Credits: %d, Remaining CPU Time: %d ms\n", 
               scheduler->running->name, scheduler->running->credits, scheduler->running->leftCPUms);
    }
    else
    {
        printf("Running Process: None\n");
    }

    // Exibe a fila de prontos
    if (scheduler->readyQueue->size > 0)
    {
        printReadyQueue(scheduler->readyQueue);
    }
    else
    {
        printf("Ready Queue is empty.\n");
    }

    // Exibe a fila de bloqueados
    if (scheduler->blockedQueue->size > 0)
    {
        printBlockedQueue(scheduler->blockedQueue);
    }
    else
    {
        printf("Blocked Queue is empty.\n");
    }

    // Exibe o último processo que terminou
    if (scheduler->exited != NULL)
    {
        printf("Exited Process: %s, Total CPU Time Used: %d ms\n", 
               scheduler->exited->name, scheduler->exited->totalCPUms);
    }
    
    // Forçar atualização imediata na tela
    fflush(stdout);
}