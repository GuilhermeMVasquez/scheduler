#include <stdio.h>
#include <stdlib.h>
#include "ReadyQueue.h"

ReadyQueue *initReadyQueue()
{
    ReadyQueue *queue = (ReadyQueue *)calloc(1, sizeof(ReadyQueue));

    if (queue == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for ReadyQueue.\n");
        return NULL;
    }

    return queue;
}

void addReadyProcess(ReadyQueue *queue, Process *process)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->process = process;
    newNode->next = NULL;

    if (queue->head == NULL || queue->head->process->credits < process->credits || (queue->head->process->credits == process->credits && queue->head->process->order < process->order))
    {
        newNode->next = queue->head;
        queue->head = newNode;
        queue->size++;
        return;
    }

    Node *current = queue->head;
    while (current->next != NULL && (current->next->process->credits > process->credits || (current->next->process->credits == process->credits && current->next->process->order > process->order)))
    {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
    queue->size++;
}

Process *dequeReadyProcess(ReadyQueue *queue)
{
    if (queue->head == NULL)
    {
        return NULL;
    }

    Node *response = queue->head;
    queue->head = queue->head->next;
    queue->size--;

    Process *process = response->process;
    free(response);
    return process;
}

int isZeroCredits(ReadyQueue *queue)
{
    if (queue->head == NULL)
    {
        return 0;
    }

    return queue->head->process->credits == 0;
}

ReadyQueue *setNewCredits(ReadyQueue *queue)
{
    if (queue->head == NULL)
    {
        return queue;
    }

    ReadyQueue *newQueue = initReadyQueue();
    Node *current = queue->head;
    while (current != NULL)
    {
        current->process->credits = current->process->credits / 2 + current->process->priority;
        addReadyProcess(newQueue, current->process);
        Node *temp = current;
        current = current->next;
        free(temp);
    }

    free(queue);
    return newQueue;
}

void printReadyQueue(ReadyQueue *queue)
{
    if (queue->head == NULL)
    {
        printf("ReadyQueue is empty.\n");
        return;
    }

    Node *current = queue->head;
    while (current != NULL)
    {
        printf("Process ID: %s, credits: %d, Order: %d\n", current->process->name, current->process->credits, current->process->order);
        current = current->next;
    }
}