#ifndef QUEUE_H
#define QUEUE_H

// #include <stdbool.h>
#include <stdlib.h> 

typedef struct binary_tree binary_tree;

typedef struct queue {
  binary_tree *data;
  struct queue *next;
}queue;



void enQueue (queue **front, queue **rear, binary_tree *element);
binary_tree *deQueue(queue **front, queue **rear);

#endif