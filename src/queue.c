#include "../include/queue.h"

void enQueue (queue **front, queue **rear, binary_tree *element) {
 
  // creating the new node despite the queue state
  queue *node = malloc(sizeof(queue));
  // check for memory allocation errors
  if(!node) return;

  node -> next = NULL;
  node -> data = element;

  // if queue is empty
  if(! (*front)) {
    *rear = *front = node;
  } 
  else {
    (*rear) -> next = node;
    *rear = node;
  }
};

binary_tree *deQueue(queue **front, queue **rear) {
  // if queue empty
  if(!(*front)) return NULL;
  // store the value
  binary_tree *deQueued = (*front) -> data;

  // store the front to free it later anyways
  queue *temp = *front;

  // updating front depending on the queue state (deleting front means the queue becomes empty or not)
  if(*rear == *front) *rear = *front = NULL;
  else *front = (*front) -> next;

  free(temp);

  return deQueued;
}