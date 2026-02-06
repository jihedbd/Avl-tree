
#ifndef TREE_H
#define TREE_H
#include <stdbool.h>

typedef struct binary_tree {
  struct binary_tree *left;
  struct binary_tree *right;
  int data;
  int height;
}binary_tree;

void freeTree (binary_tree *root);

//tree traversels
void inorder(binary_tree *root);
void preorder(binary_tree *root);
void postorder(binary_tree *root);
void levelOrder (binary_tree *root);

void display_tree(binary_tree *root);

int height (binary_tree *root);


binary_tree *insertAvlTree (binary_tree *root, int key);

#endif