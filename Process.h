#ifndef PROCESS_H
#define PROCESS_H

#include <stdlib.h>

typedef struct Process {
    char *name;

    unsigned int burstMs;         // CPU burst time in milliseconds
    unsigned int currentBurst;     // Current remaining burst time

    unsigned int ioMs;             // I/O time in milliseconds
    unsigned int currentIO;        // Current remaining I/O time

    unsigned int totalCPUms;       // Total CPU time required
    unsigned int leftCPUms;        // Remaining CPU time

    unsigned int order;            // Order of process arrival
    unsigned int priority;         // Process priority level

    unsigned int credits;          // Credits for priority-based scheduling
} Process;

/**
 * @brief Initialize a Process structure.
 * 
 * @param name The name of the process.
 * @param burstMS The CPU burst time in milliseconds.
 * @param ioMs The I/O time in milliseconds.
 * @param totalCPUms Total CPU time required by the process.
 * @param order Order of the process (used for scheduling).
 * @param priority Process priority level.
 * @return Process* Pointer to the newly allocated Process structure.
 */
Process *initProcess(char *name, 
                     unsigned int burstMS, 
                     unsigned int ioMs,
                     unsigned int totalCPUms,
                     unsigned int order,
                     unsigned int priority);

#endif // PROCESS_H
