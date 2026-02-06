#include "../include/tree.h"
#include "../include/queue.h"


#include <stdio.h>
#include <stdlib.h>

// number of insertions to look for imbalance factor is three if the tree is empty then two more generic (a balanced tree won't get imbalanced if one element is added)
// where to calculate the imblanace factor ? is for the newely created node parent then it's parent only three nodes are necessary for rotating
// how to know which rotating method to use : if imbalance factor is positive it's left if the imbalance factor is negative it's right then combine ll ?rr ? lr? rl?

void freeTree (binary_tree *root) {
  if(root) {
    freeTree (root -> left);
    freeTree (root -> right);
    free(root);
  }
}

// tree traversels
void preorder (binary_tree *root) {
  if(root) {
    printf("%d| ", root -> data);
    preorder(root -> left);
    preorder(root -> right);
  }
}

void inorder (binary_tree *root) {
  if(root) {
    inorder(root-> left);
    printf("%d| ", root -> data);
    inorder(root -> right);

  }
}

void postorder (binary_tree *root) {
  if(root) {
    postorder(root-> left);
    postorder(root -> right);
    printf("%d| ", root -> data);

  }
}

void levelOrder (binary_tree *root) {
  if(!root) printf("tree empty");
  
  queue *front = NULL;
  queue *rear = NULL;
  enQueue (&front, &rear, root);

  while(front)
  {
    binary_tree *node = deQueue(&front, &rear);
    if(node) printf("\n%d\n", node -> data);
    else printf("\n%p\n", node);
    enQueue (&front, &rear, node -> left);
    enQueue (&front, &rear, node -> right);
  }
  
}

int height (binary_tree *root) {
  if(!root) return 0;
  int l = height(root -> left);
  int r = height (root -> right);
  return l > r ? l + 1 : r + 1;
}

int NodeHeight(binary_tree *node) {

int hl,hr;
// -1 for null children since we start the height from zero onwards
hl = node && node -> left  ? node -> left -> height : -1;
hr = node && node -> right ? node -> right -> height : -1;

return hl > hr ? hl + 1 : hr + 1;
}

binary_tree *llRotation (binary_tree *root, binary_tree *prev) {

  binary_tree *newRoot = root -> left;
  binary_tree *childRight = newRoot -> right;

  // update both root and newRoot l and r pointers
  newRoot -> right = root;
  root -> left = childRight;

  if(root != prev)
  {
    if(prev -> left == root) prev -> left = newRoot;
    else prev -> right = newRoot;
  }

  // height update (only update rotated nodes parent's and the newRoot is last one to update as his height is dependent on the updated height's of his new children)   
  root -> height = NodeHeight(root);
  newRoot -> height = NodeHeight(newRoot);

  return newRoot;
}


binary_tree *rrRotation (binary_tree *root, binary_tree *prev) {
  binary_tree *newRoot = root -> right;
  binary_tree *childLeft = newRoot -> left;

  newRoot -> left = root;
  root -> right = childLeft;
  
  if(root != prev)
  {
    if(prev -> left == root) prev -> left = newRoot;
    else prev -> right = newRoot;
  }

  // height update
  root -> height = NodeHeight(root);
  newRoot -> height = NodeHeight(newRoot);

  return newRoot;
}

// last element basically is the new root which is new root = (root -> left) -> right 
// then the previous root becomes on the left and child becomes on the right of the new root
// then the new root child's will take the place were the previous nodes were linked to each other for example
//  newRoot (root->left) -> right so it's left go to the child that was on root's right on the node that was  linked to newRoot 
// same goes for root but this time for the node linked with child takes newRoot rightchild
binary_tree *lrRotation (binary_tree *root, binary_tree *prev) {
 
  binary_tree *child = root -> left;
  binary_tree *newRoot = child -> right;
  
  // saving the new root data
  binary_tree *newRootLeft = newRoot -> left;
  binary_tree *newRootRight = newRoot -> right;

  // updating the new root l and r pointers
  newRoot -> left = child;
  newRoot -> right = root;

  // setting the new root children
  child -> right = newRootLeft;
  root -> left = newRootRight;

  // updating the element holding the previous root to the new root
  if(root != prev)
  {
    if(prev -> left == root) prev -> left = newRoot;
    else prev -> right = newRoot;
  }

  // height update
  child -> height = NodeHeight(child);
  root -> height = NodeHeight(root);
  newRoot -> height = NodeHeight(newRoot);

  return newRoot;
}

binary_tree *rlRotation (binary_tree *root, binary_tree *prev) {
 
  binary_tree *child = root -> right;  
  binary_tree *newRoot = child -> left;

  // saving the new root data
  binary_tree *newRootLeft = newRoot -> left;
  binary_tree *newRootRight = newRoot -> right;

  // updating the new root l and r pointers child on it's left and root on it's right
  newRoot -> left = root;
  newRoot -> right = child;

  // setting the new root children
  child -> left = newRootRight;
  root -> right = newRootLeft;

  // updating the element holding the previous root to the new root
  if(root != prev)
  {
    if(prev -> left == root) prev -> left = newRoot;
    else prev -> right = newRoot;
  }

  // height update
  child -> height = NodeHeight(child);
  root -> height = NodeHeight(root);
  newRoot -> height = NodeHeight(newRoot);


  return newRoot;
}

int calculateBalanceFactor (binary_tree *node) {
  binary_tree *nodeLeft = node -> left;
  binary_tree *nodeRight = node -> right;

  int leftChildHeight = nodeLeft ? nodeLeft -> height : -1;
  int rightChildHeight = nodeRight ? nodeRight -> height : -1;

  return leftChildHeight - rightChildHeight;
}

binary_tree *createNodeAvlTree (binary_tree *curr, binary_tree *prev, int key) {
    // printf("DEBUG: Entering createNodeAvlTree with curr=%p, prev=%p, key=%d\n", (void*)curr, (void*)prev, key);

  // classic bst insertion
  if (!curr)
  {
    //this is for increasing the height of the new parent when it only has no children 
    if(prev && !(prev -> left) && !(prev -> right) ) prev -> height++;

    binary_tree *node = malloc(sizeof(binary_tree));
    node -> data = key;
    node -> height = 0;
    node -> left = node -> right = NULL;
  
    return node;
  }

  if(curr -> data < key) curr -> right = createNodeAvlTree(curr -> right, curr, key);
  else if (curr -> data > key) curr -> left = createNodeAvlTree(curr -> left, curr, key);
  
  // updating the height as needed
  // if curr != prev to prevent updating the root every time  cause root has the height of the whole tree
  // if height of the previous node which is the parent of the new parent is updated is equal to it's child curr
  //  we increase the height of the parent of the new parent aka prev cause every parent and child height diff is maximum 1
  int balanceFactor = calculateBalanceFactor(curr);

  // bst tree height balancing

  if( balanceFactor < -1 || balanceFactor > 1 )
  {
    binary_tree *child = NULL;
    int childBalanceFactor;

    if(balanceFactor < 0)
    {
      child = curr -> right;
      childBalanceFactor = calculateBalanceFactor(child);
      if (childBalanceFactor < 0) curr = rrRotation(curr, prev);
      else curr = rlRotation(curr, prev);
    }
    else
    {
      child = curr -> left;
      childBalanceFactor = calculateBalanceFactor(child);
      if (childBalanceFactor < 0) curr = lrRotation(curr, prev);
      else curr = llRotation(curr, prev);
    }
  }
  
  // this jumps after the balancing check cause we don't want to update a parent based on a dibalanced child
  int height = curr -> height; 
  if(curr != prev && height == prev -> height) prev -> height++;

  return curr;
}

binary_tree *insertAvlTree (binary_tree *root, int key) {

  return createNodeAvlTree(root, root, key);

}