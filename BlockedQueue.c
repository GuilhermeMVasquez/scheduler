#include <stdio.h>
#include <stdlib.h>
#include "BlockedQueue.h"



BlockedQueue *initBlockedQueue( unsigned int capacity ) { 

    BlockedQueue *queue = (BlockedQueue *)malloc(sizeof(BlockedQueue));
    // casenffor some reason the malloc fail
    if( queue == NULL ) {
        fprintf(stderr, "Failed to allocate memory for BlockedQueue.\n");
        return NULL;
    }

    queue->capacity = capacity;
    queue->size = 0;
    return queue;
}

int addBlockedProcess( BlockedQueue *queue, Process *process ) {

    if( queue->size == queue->capacity )  
        return -1;

    if(queue->processes == NULL) 
        queue->processes = ( Process** )malloc(queue->capacity * sizeof(Process *));
    
    queue->processes[queue->size++] = process;
    if( queue->processes == NULL ) // case the malloc fail
        return -1;

    return 0;

}


Process *removeBlockedProcess( BlockedQueue *queue ) {


    if(queue->size == 0)
        return NULL;

    Process *process = queue->processes[0];

    for( unsigned int i = 1; i < queue->size; i++ ) {
        queue->processes[i - 1] = queue->processes[i];
    }

    queue->size--;

    return process;
}

// Need to verify if the use of free is correct.
void freeBlockedQueue( BlockedQueue *queue ) {

    if( queue->size == 0 )
        return;
    
    if( queue->processes != NULL ) {
        for( unsigned int i = 0; i < queue->size; i++ ) {
            free(queue->processes[i]);
        }

        free(queue->processes);
    }
        
    free( queue );
}

