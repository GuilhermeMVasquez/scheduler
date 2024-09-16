#ifndef BLOCKEDQUEUE_H
#define BLOCKEDQUEUE_H

#include "Process.h"

// IMPORTANT: SIZES MUST BE = (A POWER OF 2)-1
#define BLOCKED_QUEUE_SIZE 255

typedef struct BlockedQueueInnerLinkedList BQInnerLinkedList;

struct BlockedQueueInnerLinkedList {
    BQInnerLinkedList *next; // Next element in the linked list
    Process *process;        // Pointer to the blocked process
    unsigned int key;        // The key (time in ms to be released by IO)
};

typedef struct BlockedQueue {
    BQInnerLinkedList *keys[BLOCKED_QUEUE_SIZE];  // Array of processes that are blocked (waiting for I/O)
    unsigned int size;                            // Amount of processes inside the structure
} BlockedQueue;

BlockedQueue *initBlockedQueue();

/*
Adiciona um processo na fila de bloqueados
*/
void addBlockedProcess(BlockedQueue *queue, Process *process, unsigned int msToBeRemoved);

/*
Tenta retirar um processo para um dado milissegundo(ms) atual.
Caso retorne NULL, não há processos no MS informado.
Caso retorne um processo, pode ainda haver outro processo no mesmo MS,
sendo correto mais uma vez a função.
*/
Process *dequeBlockedProcess(BlockedQueue *queue, unsigned int currentMS);

#endif // BLOCKEDQUEUE_H
