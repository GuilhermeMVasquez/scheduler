#ifndef BLOCKEDQUEUE_H
#define BLOCKEDQUEUE_H

#include "Process.h"

typedef struct BlockedQueue {
    Process **processes;  // Array of processes that are blocked (waiting for I/O)
    unsigned int size;    // Number of processes in the blocked queue
    unsigned int capacity; // Capacity of the blocked queue
} BlockedQueue;

/**
 * @brief Initialize a BlockedQueue structure.
 * 
 * @param capacity The maximum number of blocked processes that can be in the queue.
 * @return BlockedQueue* Pointer to the newly allocated BlockedQueue structure.
 */
BlockedQueue *initBlockedQueue(unsigned int capacity);

/**
 * @brief Add a process to the blocked queue.
 * 
 * @param queue The BlockedQueue to which the process will be added.
 * @param process The process to add (process waiting for I/O).
 * @return int 0 on success, -1 on failure (e.g., if queue is full).
 */
int addBlockedProcess(BlockedQueue *queue, Process *process);

/**
 * @brief Remove a process from the blocked queue.
 * 
 * @param queue The BlockedQueue from which to remove a process.
 * @return Process* Pointer to the removed process, or NULL if the queue is empty.
 */
Process *removeBlockedProcess(BlockedQueue *queue);

/**
 * @brief Free the memory allocated for the BlockedQueue.
 * 
 * @param queue The BlockedQueue to be freed.
 */
void freeBlockedQueue(BlockedQueue *queue);

#endif // BLOCKEDQUEUE_H
