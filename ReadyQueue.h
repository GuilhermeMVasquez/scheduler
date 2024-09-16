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

void addReadyProcess(ReadyQueue *queue, Process *process);

Process *dequeReadyProcess(ReadyQueue *queue);