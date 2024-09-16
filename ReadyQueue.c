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

    if (queue->head == NULL || queue->head->process->priority < process->priority || (queue->head->process->priority == process->priority && queue->head->process->order < process->order))
    {
        newNode->next = queue->head;
        queue->head = newNode;
        queue->size++;
        return;
    }

    Node *current = queue->head;
    while (current->next != NULL && (current->next->process->priority > process->priority || (current->next->process->priority == process->priority && current->next->process->order > process->order)))
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
        printf("Process ID: %s, Priority: %d, Order: %d\n", current->process->name, current->process->priority, current->process->order);
        current = current->next;
    }
}