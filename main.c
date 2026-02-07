#include "include/tree.h" 
#include <stdlib.h>
#include <stdio.h>


int main () {
  int test_array [] = {30, 20, 10, 25, 5, 15, 40, 50, 35, 45, 60, 55, 1, 2, 3};
  int length = 14;
  binary_tree *root = NULL;

  for (int *i = test_array; i < test_array + length; i++) root = insertAvlTree(root, *i);

  printf("========================= inorder traversel ================================================\n");
  inorder(root);
  printf("\n============== inorder traversel should be =================================================");
  printf("\n1, 2, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60\n");
  
  return 0;
}



/*

        Final AVL Tree Structure:
        
               30
             /    \
           10      45
          /  \    /  \
         2   20  40  50
        / \  / \     / \
       1  3 5  25   35 55
                \        \
                15       60
        
        Height: 4 (balanced)
*/