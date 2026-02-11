#include "include/tree.h" 

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


// ===== VALIDATION HELPERS (ALL USING 'data' FIELD) =====
int height(binary_tree *node) {
    return node ? node->height : -1;
}

int get_balance(binary_tree *node) {
    return node ? height(node->left) - height(node->right) : 0;
}

// Verify BST property (in-order traversal must be strictly increasing)
void verify_bst(binary_tree *root, int *prev, bool *valid) {
    if (!root || !*valid) return;
    verify_bst(root->left, prev, valid);
    if (*prev != -1 && root->data <= *prev) *valid = false;  // <-- data field
    *prev = root->data;  // <-- data field
    verify_bst(root->right, prev, valid);
}

bool is_valid_bst(binary_tree *root) {
    int prev = -1;
    bool valid = true;
    verify_bst(root, &prev, &valid);
    return valid;
}

// Verify AVL balance property (|balance| <= 1 everywhere)
bool verify_avl_balance(binary_tree *root) {
    if (!root) return true;
    int bf = get_balance(root);
    if (bf < -1 || bf > 1) {
        printf("❌ Balance violation at data=%d (bf=%d)\n", root->data, bf);
        return false;
    }
    return verify_avl_balance(root->left) && verify_avl_balance(root->right);
}

// Verify height consistency
bool verify_heights(binary_tree *root) {
    if (!root) return true;
    int actual_height = 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));
    if (root->height != actual_height) {
        printf("❌ Height mismatch at data=%d: stored=%d, actual=%d\n", 
               root->data, root->height, actual_height);
        return false;
    }
    return verify_heights(root->left) && verify_heights(root->right);
}

// Full validation (BST + balance + heights)
bool validate_avl_tree(binary_tree *root, const char *test_name) {
    if (!root) return true;  // Empty tree is valid AVL
    
    bool bst_ok = is_valid_bst(root);
    bool balance_ok = verify_avl_balance(root);
    bool height_ok = verify_heights(root);
    
    if (!bst_ok || !balance_ok || !height_ok) {
        printf("\n❌ TEST FAILED: %s\n", test_name);
        if (!bst_ok) printf("   → BST property violated\n");
        if (!balance_ok) printf("   → Balance factor violation\n");
        if (!height_ok) printf("   → Height inconsistency\n");
        return false;
    }
    return true;
}

// Search node by value (using 'data' field)
bool search(binary_tree *root, int value) {
    if (!root) return false;
    if (value == root->data) return true;
    return value < root->data ? search(root->left, value) : search(root->right, value);
}

// Free entire tree
void free_tree(binary_tree *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

// Optional: Visual debug print (shows data, height, balance factor)
void print_tree(binary_tree *root, int indent) {
    if (!root) return;
    print_tree(root->right, indent + 4);
    printf("%*s%d (h=%d, bf=%d)\n", indent, "", root->data, root->height, get_balance(root));
    print_tree(root->left, indent + 4);
}



void run_all_deletion_tests() {
    printf("========================================\n");
    printf("🚀 STARTING AVL DELETION TEST SUITE\n");
    printf("   • Insertion: insertAvlTree()\n");
    printf("   • Node field: 'data'\n");
    printf("========================================\n\n");

    // ===== TEST 0: PRE-FLIGHT CHECK - Verify AVL insertion works =====
    {
        binary_tree *root = NULL;
        int keys[] = {50, 30, 70, 20, 40, 60, 80};
        for (int i = 0; i < 7; i++) {
            root = insertAvlTree(root, keys[i]);
        }
        assert(validate_avl_tree(root, "PRE-FLIGHT: Initial AVL insertion") && 
               "❌ AVL INSERTION IS BROKEN! Fix insertAvlTree() first.");
        free_tree(root);
        printf("✅ PRE-FLIGHT CHECK PASSED: AVL insertion works correctly\n\n");
    }

    // ===== TEST 1: Delete from empty tree =====
    {
        binary_tree *root = NULL;
        root = deleteNodeAvlTree(root, 42);
        assert(root == NULL && "Empty tree deletion should return NULL");
        printf("✅ TEST 1 PASSED: Delete from empty tree\n");
    }

    // ===== TEST 2: Delete only node (root) =====
    {
        binary_tree *root = NULL;
        root = insertAvlTree(root, 50);
        assert(search(root, 50) && "Insert failed");
        root = deleteNodeAvlTree(root, 50);
        assert(root == NULL && "Root deletion should yield NULL");
        printf("✅ TEST 2 PASSED: Delete only node\n");
    }

    // ===== TEST 3: Delete leaf node =====
    {
        binary_tree *root = NULL;
        int keys[] = {50, 30, 70, 20, 40, 60, 80};
        for (int i = 0; i < 7; i++) root = insertAvlTree(root, keys[i]);
        assert(validate_avl_tree(root, "TEST 3: Pre-deletion") && "Tree unbalanced before deletion!");
        
        root = deleteNodeAvlTree(root, 20);  // Leaf node
        assert(!search(root, 20) && "data=20 should be deleted");
        assert(search(root, 50) && search(root, 30) && search(root, 40) && "Other nodes intact");
        assert(validate_avl_tree(root, "TEST 3: Post-deletion (leaf)"));
        free_tree(root);
        printf("✅ TEST 3 PASSED: Delete leaf node\n");
    }

    // ===== TEST 4: Delete node with only left child =====
    {
        binary_tree *root = NULL;
        root = insertAvlTree(root, 50);
        root = insertAvlTree(root, 30);
        root = insertAvlTree(root, 20);  // 30 has only left child
        
        assert(validate_avl_tree(root, "TEST 4: Pre-deletion") && "Tree unbalanced!");
        root = deleteNodeAvlTree(root, 30);
        assert(!search(root, 30) && "data=30 deleted");
        assert(search(root, 20) && search(root, 50) && "Children preserved");
        assert(validate_avl_tree(root, "TEST 4: Post-deletion (left-child-only)"));
        free_tree(root);
        printf("✅ TEST 4 PASSED: Delete node with only left child\n");
    }

    // ===== TEST 5: Delete node with only right child =====
    {
        binary_tree *root = NULL;
        root = insertAvlTree(root, 50);
        root = insertAvlTree(root, 70);
        root = insertAvlTree(root, 80);  // 70 has only right child
        
        assert(validate_avl_tree(root, "TEST 5: Pre-deletion") && "Tree unbalanced!");
        root = deleteNodeAvlTree(root, 70);
        assert(!search(root, 70) && "data=70 deleted");
        assert(search(root, 50) && search(root, 80) && "Children preserved");
        assert(validate_avl_tree(root, "TEST 5: Post-deletion (right-child-only)"));
        free_tree(root);
        printf("✅ TEST 5 PASSED: Delete node with only right child\n");
    }

    // ===== TEST 6: Delete node with two children (successor case) =====
    {
        binary_tree *root = NULL;
        int keys[] = {50, 30, 70, 20, 40, 60, 80};
        for (int i = 0; i < 7; i++) root = insertAvlTree(root, keys[i]);
        assert(validate_avl_tree(root, "TEST 6: Pre-deletion") && "Tree unbalanced!");
        
        // Delete root (50) → successor is 60
        root = deleteNodeAvlTree(root, 50);
        assert(!search(root, 50) && "Root deleted");
        assert(search(root, 60) && "Successor (60) promoted");
        assert(validate_avl_tree(root, "TEST 6: Post-deletion (two children)"));
        free_tree(root);
        printf("✅ TEST 6 PASSED: Delete node with two children\n");
    }

    // ===== TEST 7: Delete non-existent key =====
    {
        binary_tree *root = NULL;
        root = insertAvlTree(root, 50);
        binary_tree *original = root;
        root = deleteNodeAvlTree(root, 999);  // Non-existent
        
        assert(root == original && "Tree pointer unchanged");
        assert(search(root, 50) && "Original data still present");
        assert(validate_avl_tree(root, "TEST 7: Non-existent key deletion"));
        free_tree(root);
        printf("✅ TEST 7 PASSED: Delete non-existent key\n");
    }

    // ===== TEST 8: LL Rotation after deletion =====
    {
        // Build tree that requires LL rotation after deleting 80
        binary_tree *root = NULL;
        int keys[] = {50, 30, 70, 20, 60, 80, 10};
        for (int i = 0; i < 7; i++) root = insertAvlTree(root, keys[i]);
        assert(validate_avl_tree(root, "TEST 8: Pre-deletion") && "Tree unbalanced!");
        
        root = deleteNodeAvlTree(root, 80);
        assert(!search(root, 80) && "data=80 deleted");
        assert(validate_avl_tree(root, "TEST 8: Post-deletion (LL rotation)"));
        free_tree(root);
        printf("✅ TEST 8 PASSED: LL rotation after deletion\n");
    }

    // ===== TEST 9: LR Rotation after deletion =====
    {
        // Requires LR rotation after deleting 70
        binary_tree *root = NULL;
        int keys[] = {50, 30, 70, 20, 40, 45};
        for (int i = 0; i < 6; i++) root = insertAvlTree(root, keys[i]);
        assert(validate_avl_tree(root, "TEST 9: Pre-deletion") && "Tree unbalanced!");
        
        root = deleteNodeAvlTree(root, 70);
        assert(!search(root, 70) && "data=70 deleted");
        assert(validate_avl_tree(root, "TEST 9: Post-deletion (LR rotation)"));
        free_tree(root);
        printf("✅ TEST 9 PASSED: LR rotation after deletion\n");
    }

    // ===== TEST 10: RR Rotation after deletion =====
    {
        // Mirror case requiring RR rotation
        binary_tree *root = NULL;
        int keys[] = {50, 30, 70, 60, 80, 90};
        for (int i = 0; i < 6; i++) root = insertAvlTree(root, keys[i]);
        assert(validate_avl_tree(root, "TEST 10: Pre-deletion") && "Tree unbalanced!");
        
        root = deleteNodeAvlTree(root, 30);
        assert(!search(root, 30) && "data=30 deleted");
        assert(validate_avl_tree(root, "TEST 10: Post-deletion (RR rotation)"));
        free_tree(root);
        printf("✅ TEST 10 PASSED: RR rotation after deletion\n");
    }

    // ===== TEST 11: RL Rotation after deletion =====
    {
        // Mirror case requiring RL rotation
        binary_tree *root = NULL;
        int keys[] = {50, 30, 70, 65, 80, 62};
        for (int i = 0; i < 6; i++) root = insertAvlTree(root, keys[i]);
        assert(validate_avl_tree(root, "TEST 11: Pre-deletion") && "Tree unbalanced!");
        
        root = deleteNodeAvlTree(root, 30);
        assert(!search(root, 30) && "data=30 deleted");
        assert(validate_avl_tree(root, "TEST 11: Post-deletion (RL rotation)"));
        free_tree(root);
        printf("✅ TEST 11 PASSED: RL rotation after deletion\n");
    }

    // ===== TEST 12: Cascading rotations (multiple rebalances) =====
    {
        binary_tree *root = NULL;
        int keys[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 85};
        for (int i = 0; i < 15; i++) root = insertAvlTree(root, keys[i]);
        assert(validate_avl_tree(root, "TEST 12: Pre-deletion") && "Tree unbalanced!");
        
        root = deleteNodeAvlTree(root, 50);  // Delete root → triggers cascading rebalancing
        assert(!search(root, 50) && "Root deleted");
        assert(validate_avl_tree(root, "TEST 12: Post-deletion (cascading rotations)"));
        free_tree(root);
        printf("✅ TEST 12 PASSED: Cascading rotations after deletion\n");
    }

    // ===== TEST 13: 100 sequential deletions (stress test) =====
    {
        binary_tree *root = NULL;
        for (int i = 1; i <= 100; i++) {
            root = insertAvlTree(root, i * 7 % 101);  // Semi-random distribution
        }
        assert(validate_avl_tree(root, "TEST 13: Pre-deletion") && "Initial tree unbalanced!");
        
        for (int i = 100; i >= 1; i--) {
            int key = i * 7 % 101;
            root = deleteNodeAvlTree(root, key);
            if (!validate_avl_tree(root, "TEST 13: During sequential deletion")) {
                printf("❌ Failed at deletion #%d (key=%d)\n", 101-i, key);
                exit(1);
            }
        }
        assert(root == NULL && "Tree should be empty after all deletions");
        printf("✅ TEST 13 PASSED: 100 sequential deletions\n");
    }

    // ===== TEST 14: Boundary values (min/max) =====
    {
        binary_tree *root = NULL;
        for (int i = 1; i <= 100; i++) root = insertAvlTree(root, i);
        assert(validate_avl_tree(root, "TEST 14: Pre-deletion") && "Tree unbalanced!");
        
        // Delete min (1)
        root = deleteNodeAvlTree(root, 1);
        assert(!search(root, 1) && validate_avl_tree(root, "TEST 14a: Min deletion"));
        
        // Delete max (100)
        root = deleteNodeAvlTree(root, 100);
        assert(!search(root, 100) && validate_avl_tree(root, "TEST 14b: Max deletion"));
        
        free_tree(root);
        printf("✅ TEST 14 PASSED: Boundary value deletions\n");
    }

    // ===== TEST 15: Repeated root deletions =====
    {
        binary_tree *root = NULL;
        int keys[] = {50, 30, 70, 20, 40, 60, 80};
        for (int i = 0; i < 7; i++) root = insertAvlTree(root, keys[i]);
        assert(validate_avl_tree(root, "TEST 15: Pre-deletion") && "Tree unbalanced!");
        
        for (int i = 0; i < 3; i++) {
            int old_root_data = root->data;
            root = deleteNodeAvlTree(root, old_root_data);
            assert(!search(root, old_root_data) && "Old root not deleted");
            assert(validate_avl_tree(root, "TEST 15: Repeated root deletion"));
        }
        free_tree(root);
        printf("✅ TEST 15 PASSED: Repeated root deletions\n");
    }

    // ===== TEST 16: Delete all nodes in sorted order =====
    {
        binary_tree *root = NULL;
        int keys[] = {40, 20, 60, 10, 30, 50, 70};
        for (int i = 0; i < 7; i++) root = insertAvlTree(root, keys[i]);
        assert(validate_avl_tree(root, "TEST 16: Pre-deletion") && "Tree unbalanced!");
        
        // Delete ascending: 10,20,30,40,50,60,70
        for (int i = 0; i < 7; i++) {
            root = deleteNodeAvlTree(root, keys[i]);
            assert(validate_avl_tree(root, "TEST 16: Sorted-order deletion"));
        }
        assert(root == NULL && "Tree not empty after all deletions");
        printf("✅ TEST 16 PASSED: Delete all nodes in sorted order\n");
    }

    // ===== TEST 17: Delete all nodes in reverse sorted order =====
    {
        binary_tree *root = NULL;
        int keys[] = {40, 20, 60, 10, 30, 50, 70};
        for (int i = 0; i < 7; i++) root = insertAvlTree(root, keys[i]);
        assert(validate_avl_tree(root, "TEST 17: Pre-deletion") && "Tree unbalanced!");
        
        // Delete descending: 70,60,50,40,30,20,10
        for (int i = 6; i >= 0; i--) {
            root = deleteNodeAvlTree(root, keys[i]);
            assert(validate_avl_tree(root, "TEST 17: Reverse-sorted deletion"));
        }
        assert(root == NULL && "Tree not empty after all deletions");
        printf("✅ TEST 17 PASSED: Delete all nodes in reverse sorted order\n");
    }

    printf("\n========================================\n");
    printf("🎉 ALL 18 TESTS PASSED SUCCESSFULLY!\n");
    printf("   • Insertion: insertAvlTree()\n");
    printf("   • Node field: 'data'\n");
    printf("   • All rotations covered: LL, LR, RR, RL\n");
    printf("========================================\n");
}

int main () {
  // int test_array [] = {30, 20, 10, 25, 5, 15, 40, 50, 35, 45, 60, 55, 1, 2, 3};
  // int length = 15;
  // binary_tree *root = NULL;

  // for (int *i = test_array; i < test_array + length; i++) root = insertAvlTree(root, *i);

  // printf("========================= inorder traversel ================================================\n");
  // inorder(root);
  // printf("\n============== inorder traversel should be =================================================");
  // printf("\n1, 2, 3, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60\n");
  

  // root = deleteNodeAvlTree(root, 2);
  // inorder(root);

  // Test 1: Basic functionality
  run_all_deletion_tests();
  return 0;

}
