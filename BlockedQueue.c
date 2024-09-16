#include <stdio.h>
#include <stdlib.h>
#include "BlockedQueue.h"

/*
IMPORTANT: The current implementation uses a HashMap, in which the 
hash is just using the unsigned integer itself. This is fine since time 
will always increase, so not many collisions are expected.
*/

BlockedQueue *initBlockedQueue()
{ 
    BlockedQueue *queue = (BlockedQueue *)calloc(1, sizeof(BlockedQueue));

    if( queue == NULL) {
        fprintf(stderr, "Failed to allocate memory for BlockedQueue.\n");
        return NULL;
    }

    return queue;
}

void addBlockedProcess(BlockedQueue *queue, Process *process, unsigned int msToBeRemoved)
{
    unsigned int position = msToBeRemoved & BLOCKED_QUEUE_SIZE;

    BQInnerLinkedList *newNode = (BQInnerLinkedList *) malloc(sizeof(BQInnerLinkedList));
    newNode->key = msToBeRemoved;
    newNode->next = NULL;
    newNode->process = process;

    BQInnerLinkedList *nodeAtPosition = queue->keys[position];

    if (nodeAtPosition == NULL) { // Simplemente insere o nodo na posição
        queue->keys[position] = newNode;
        return;
    }

    while (nodeAtPosition->next != NULL) // Caminha até o próximo nodo não existir
    {
        nodeAtPosition = nodeAtPosition->next;
    }

    nodeAtPosition->next = newNode;
    return;
}

Process *dequeBlockedProcess(BlockedQueue *queue, unsigned int currentMS)
{
    unsigned int position = currentMS & BLOCKED_QUEUE_SIZE;

    BQInnerLinkedList *nodeAtPosition = queue->keys[position];


    if (nodeAtPosition == NULL) { // Verifica se é nulo
        return NULL;
    }

    if (nodeAtPosition->key == currentMS) { // se o primeiro nodo já for da chave certa
        queue->keys[position] = nodeAtPosition->next;
        return nodeAtPosition->process;
    }

    // Caso o primeiro nodo não seja da chave certa,
    // Busca o nodo com a chave certa
    while (nodeAtPosition->next != NULL)
    {
        if (nodeAtPosition->next->key == currentMS) {
            BQInnerLinkedList *response = nodeAtPosition->next->process;
            nodeAtPosition->next = nodeAtPosition->next->next;
            return response;
        }
    }
    
    return NULL;
}