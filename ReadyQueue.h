#ifndef READYQUEUE_H
#define READYQUEUE_H

#include "Process.h"

typedef struct Node
{
    Process *process;
    struct Node *next;
} Node;

typedef struct ReadyQueue
{
    Node *head;
    unsigned int size;
} ReadyQueue;

ReadyQueue *initReadyQueue();

void addReadyProcess(ReadyQueue *queue, Process *process);

Process *dequeReadyProcess(ReadyQueue *queue);

void printReadyQueue(ReadyQueue *queue);

#endif // READYQUEUE_H