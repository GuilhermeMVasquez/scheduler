#ifndef SCHEDULERQUEUE_H
#define SCHEDULERQUEUE_H

#include "Process.h"

typedef struct SchedulerQueue {
    Process **processes;  // Array of processes
    unsigned int size;    // Number of processes in the queue
    unsigned int capacity; // Capacity of the queue
} SchedulerQueue;

/**
 * @brief Initialize a SchedulerQueue structure.
 * 
 * @param processes An array of processes to be added to the queue.
 * @param capacity The maximum number of processes that can be in the queue.
 * @return SchedulerQueue* Pointer to the newly allocated SchedulerQueue structure.
 */
SchedulerQueue *initSchedulerQueue(unsigned int capacity);

/**
 * @brief Add a process to the queue.
 * 
 * @param queue The SchedulerQueue to which the process will be added.
 * @param process The process to add.
 * @return int 0 on success, -1 on failure (e.g., if queue is full).
 */
int addProcess(SchedulerQueue *queue, Process *process);

/**
 * @brief Remove a process from the queue.
 * 
 * @param queue The SchedulerQueue from which to remove a process.
 * @return Process* Pointer to the removed process, or NULL if the queue is empty.
 */
Process *removeProcess(SchedulerQueue *queue);

/**
 * @brief Free the memory allocated for the SchedulerQueue.
 * 
 * @param queue The SchedulerQueue to be freed.
 */
void freeSchedulerQueue(SchedulerQueue *queue);

#endif // SCHEDULERQUEUE_H