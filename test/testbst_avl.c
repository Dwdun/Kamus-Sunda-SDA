/* File    : testbst_avl.c                                  */
/* Desk    : Test Unit untuk AVL-BST (bst_avl.h/c)         */
/* Oleh    : Muhammad Faqih Shiam (251524023)              */
/*           Rajbi Muhammad Nizar (251524027)              */
/* Tgl     : 2024                                           */
/* Mk      : Struktur Data dan Algoritma                   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/bst_avl.h"

/* ============================================================
    HELPER FUNCTION
   ============================================================ */

void PrintSeparator(char *title) {
    printf("\n");
    printf("+--------------------------------------------------------+\n");
    printf("| %-54s |\n", title);
    printf("+--------------------------------------------------------+\n");
}

void PrintTestCase(int no, char *description) {
    printf("\n[Test Case %d] %s\n", no, description);
    printf("-----------------------------------------------------\n");
}

void PrintResult(boolean passed) {
    if (passed) {
        printf("[V] PASSED\n");
    } else {
        printf("[X] FAILED\n");
    }
}

/* ============================================================
   TEST 1: BASIC INSERTION & SEARCH
   ============================================================ */

void TestBasicInsertionSearch() {
    PrintTestCase(1, "Basic Insertion & Search");
    
    AVLTree tree;
    CreateEmptyBST(&tree);
    
    /* Test Case 1.1: Insert ke tree kosong */
    printf("\n1.1. Insert ke tree kosong\n");
    char *sinonim1[] = {"senang", "gembira", "bahagia"};
    tree = InsertBST(tree, "bagja", "senang", sinonim1);
    printf("  Inserted: bagja -> senang\n");
    PrintResult(tree != Nil);
    
    /* Test Case 1.2: Insert lebih banyak */
    printf("\n1.2. Insert kata tambahan\n");
    char *sinonim2[] = {"halus", "sopan", "santun"};
    tree = InsertBST(tree, "lemes", "halus", sinonim2);
    
    char *sinonim3[] = {"besar", "luas", ""};
    tree = InsertBST(tree, "ageng", "besar", sinonim3);
    
    printf("  Inserted: lemes -> halus\n");
    printf("  Inserted: ageng -> besar\n");
    PrintResult(NbElmtBST(tree) == 3);
    
    /* Test Case 1.3: Search existing key */
    printf("\n1.3. Search existing key\n");
    addressAVL found = SearchBST(tree, "bagja");
    if (found != Nil) {
        printf("  Found: %s -> %s\n", found->key, found->terjemahan_id);
        PrintResult(strcmp(found->terjemahan_id, "senang") == 0);
    } else {
        PrintResult(false);
    }
    
    /* Test Case 1.4: Search non-existing key */
    printf("\n1.4. Search non-existing key\n");
    found = SearchBST(tree, "tidak_ada");
    printf("  Result: %s\n", (found == Nil) ? "Not found (EXPECTED)" : "Found (ERROR)");
    PrintResult(found == Nil);
    
    /* Test Case 1.5: SearchExistBST function */
    printf("\n1.5. SearchExistBST function\n");
    boolean exist1 = SearchExistBST(tree, "lemes");
    boolean exist2 = SearchExistBST(tree, "xyz");
    printf("  SearchExistBST(tree, \"lemes\"): %s\n", exist1 ? "true" : "false");
    printf("  SearchExistBST(tree, \"xyz\"): %s\n", exist2 ? "true" : "false");
    PrintResult(exist1 == true && exist2 == false);
    
    /* Cleanup */
    DestroyBST(&tree);
    printf("\nTreenya sudah di-cleanup.\n");
}

/* ============================================================
   TEST 2: INSERTION DENGAN BALANCING (AVL)
   ============================================================ */

void TestAVLBalancing() {
    PrintTestCase(2, "AVL Balancing (Left-Left, Right-Right, LR, RL cases)");
    
    AVLTree tree;
    CreateEmptyBST(&tree);
    
    /* Test Case 2.1: Left-Left case (perlu rotasi kanan) */
    printf("\n2.1. Left-Left case (insert ascending order)\n");
    char *empty_syn[] = {"", "", ""};
    
    tree = InsertBST(tree, "charlie", "c", empty_syn);
    tree = InsertBST(tree, "bob", "b", empty_syn);
    tree = InsertBST(tree, "alice", "a", empty_syn);  /* Trigger LL rotation */
    
    printf("  Insert order: charlie -> bob -> alice\n");
    printf("  Expected: rotasi kanan terjadi\n");
    
    addressAVL root = tree;
    printf("  Root setelah rotasi: %s (expected: bob)\n", root->key);
    PrintResult(strcmp(root->key, "bob") == 0);
    
    /* Test Case 2.2: Check tree properties */
    printf("\n2.2. Check tree properties (BST & AVL valid)\n");
    boolean valid_bst = IsValidBST(tree);
    boolean valid_avl = IsValidAVL(tree);
    printf("  IsValidBST: %s\n", valid_bst ? "true" : "false");
    printf("  IsValidAVL: %s\n", valid_avl ? "true" : "false");
    PrintResult(valid_bst && valid_avl);
    
    /* Test Case 2.3: Right-Right case */
    printf("\n2.3. Right-Right case (insert descending order)\n");
    DestroyBST(&tree);
    CreateEmptyBST(&tree);
    
    tree = InsertBST(tree, "alice", "a", empty_syn);
    tree = InsertBST(tree, "bob", "b", empty_syn);
    tree = InsertBST(tree, "charlie", "c", empty_syn);  /* Trigger RR rotation */
    
    printf("  Insert order: alice -> bob -> charlie\n");
    root = tree;
    printf("  Root setelah rotasi: %s (expected: bob)\n", root->key);
    PrintResult(strcmp(root->key, "bob") == 0 && IsValidAVL(tree));
    
    /* Test Case 2.4: More complex insertion */
    printf("\n2.4. Complex insertion (10 nodes)\n");
    DestroyBST(&tree);
    CreateEmptyBST(&tree);
    
    char *keys[] = {"m", "d", "v", "c", "f", "s", "x", "b", "e", "z"};
    int i; for (i = 0; i < 10; i++) {
        tree = InsertBST(tree, keys[i], keys[i], empty_syn);
    }
    
    printf("  Inserted 10 nodes\n");
    printf("  Total nodes: %d\n", NbElmtBST(tree));
    printf("  Tree is valid AVL: %s\n", IsValidAVL(tree) ? "true" : "false");
    PrintResult(NbElmtBST(tree) == 10 && IsValidAVL(tree));
    
    /* Cleanup */
    DestroyBST(&tree);
}

/* ============================================================
   TEST 3: DELETION
   ============================================================ */

void TestDeletion() {
    PrintTestCase(3, "Deletion & Balancing");
    
    AVLTree tree;
    CreateEmptyBST(&tree);
    
    /* Setup: Insert beberapa kata */
    printf("\n3.1. Setup: Insert 5 nodes\n");
    char *empty_syn[] = {"", "", ""};
    
    tree = InsertBST(tree, "m", "m", empty_syn);
    tree = InsertBST(tree, "d", "d", empty_syn);
    tree = InsertBST(tree, "v", "v", empty_syn);
    tree = InsertBST(tree, "c", "c", empty_syn);
    tree = InsertBST(tree, "f", "f", empty_syn);
    
    printf("  Inserted: m, d, v, c, f\n");
    printf("  Total: %d\n", NbElmtBST(tree));
    
    /* Test Case 3.2: Delete leaf node */
    printf("\n3.2. Delete leaf node (c)\n");
    tree = DeleteBST(tree, "c");
    printf("  After delete 'c': %d nodes\n", NbElmtBST(tree));
    PrintResult(NbElmtBST(tree) == 4 && SearchExistBST(tree, "c") == false);
    
    /* Test Case 3.3: Delete node dengan 1 child */
    printf("\n3.3. Delete node dengan 1 child\n");
    tree = InsertBST(tree, "c", "c", empty_syn);  /* Re-insert for setup */
    tree = DeleteBST(tree, "f");
    printf("  After delete 'f': %d nodes\n", NbElmtBST(tree));
    PrintResult(NbElmtBST(tree) == 4);
    
    /* Test Case 3.4: Delete node dengan 2 children */
    printf("\n3.4. Delete node dengan 2 children (d)\n");
    tree = DeleteBST(tree, "d");
    printf("  After delete 'd': %d nodes\n", NbElmtBST(tree));
    printf("  Tree still valid: %s\n", IsValidAVL(tree) ? "true" : "false");
    PrintResult(NbElmtBST(tree) == 3 && IsValidAVL(tree));
    
    /* Test Case 3.5: Delete non-existing */
    printf("\n3.5. Delete non-existing key\n");
    int before = NbElmtBST(tree);
    tree = DeleteBST(tree, "tidak_ada");
    int after = NbElmtBST(tree);
    printf("  Nodes before: %d, after: %d\n", before, after);
    PrintResult(before == after);
    
    /* Cleanup */
    DestroyBST(&tree);
}

/* ============================================================
   TEST 4: TRAVERSAL & STATISTIK
   ============================================================ */

void TestTraversalAndStatistik() {
    PrintTestCase(4, "Traversal & Statistik");
    
    AVLTree tree;
    CreateEmptyBST(&tree);
    
    /* Setup */
    printf("\n4.1. Setup: Insert 5 nodes\n");
    char *empty_syn[] = {"", "", ""};
    
    tree = InsertBST(tree, "m", "m", empty_syn);
    tree = InsertBST(tree, "d", "d", empty_syn);
    tree = InsertBST(tree, "v", "v", empty_syn);
    tree = InsertBST(tree, "c", "c", empty_syn);
    tree = InsertBST(tree, "x", "x", empty_syn);
    
    /* Test Case 4.2: Inorder Traversal */
    printf("\n4.2. Inorder Traversal (should be sorted)\n");
    printf("  Output: ");
    InorderBST(tree);
    
    /* Test Case 4.3: NbElmtBST */
    printf("\n4.3. NbElmtBST\n");
    printf("  Total nodes: %d (expected: 5)\n", NbElmtBST(tree));
    PrintResult(NbElmtBST(tree) == 5);
    
    /* Test Case 4.4: NbDaunBST */
    printf("\n4.4. NbDaunBST (leaf nodes)\n");
    printf("  Leaf nodes: %d\n", NbDaunBST(tree));
    PrintResult(NbDaunBST(tree) > 0);
    
    /* Test Case 4.5: LevelBST */
    printf("\n4.5. LevelBST\n");
    int level_m = LevelBST(tree, "m");
    int level_c = LevelBST(tree, "c");
    int level_xyz = LevelBST(tree, "xyz");
    printf("  Level of 'm': %d\n", level_m);
    printf("  Level of 'c': %d\n", level_c);
    printf("  Level of 'xyz' (not found): %d\n", level_xyz);
    PrintResult(level_m > 0 && level_xyz == 0);
    
    /* Cleanup */
    DestroyBST(&tree);
}

/* ============================================================
   TEST 5: SINONIM HANDLING
   ============================================================ */

void TestSinonimHandling() {
    PrintTestCase(5, "Sinonim (Thesaurus) Handling");
    
    AVLTree tree;
    CreateEmptyBST(&tree);
    
    /* Test Case 5.1: Insert dengan sinonim */
    printf("\n5.1. Insert dengan sinonim\n");
    char *sinonim_bagja[] = {"mulia", "gemilang", "bahagia"};
    tree = InsertBST(tree, "bagja", "senang", sinonim_bagja);
    
    printf("  Inserted: bagja -> senang (sinonim: mulia, gemilang, bahagia)\n");
    
    /* Test Case 5.2: Retrieve sinonim */
    printf("\n5.2. Retrieve sinonim dari node\n");
    addressAVL node = SearchBST(tree, "bagja");
    if (node != Nil) {
        printf("  Sinonim untuk 'bagja':\n");
        int i;
        for (i = 0; i < MaxSinonim; i++) {
            if (node->sinonim[i] != Nil) {
                printf("    [%d] %s\n", i+1, node->sinonim[i]);
            }
        }
        PrintResult(node->sinonim[0] != Nil);
    } else {
        PrintResult(false);
    }
    
    /* Test Case 5.3: Insert dengan partial sinonim */
    printf("\n5.3. Insert dengan partial sinonim\n");
    char *sinonim_lemes[] = {"halus", "", ""};
    tree = InsertBST(tree, "lemes", "halus", sinonim_lemes);
    
    node = SearchBST(tree, "lemes");
    printf("  Sinonim untuk 'lemes':\n");
    int i;
    for (i = 0; i < MaxSinonim; i++) {
        if (node->sinonim[i] != Nil) {
            printf("    [%d] %s\n", i+1, node->sinonim[i]);
        } else {
            printf("    [%d] (empty)\n", i+1);
        }
    }
    PrintResult(node->sinonim[0] != Nil && node->sinonim[1] == Nil);
    
    /* Cleanup */
    DestroyBST(&tree);
}

/* ============================================================
   MAIN TEST RUNNER
   ============================================================ */

int main() {
    printf("\n");
    printf("+-------------------------------------------------------+\n");
    printf("|        TEST UNIT: AVL-BST (bst_avl.h/c)               |\n");
    printf("|    Aplikasi Nyunda - Kamus Sunda Interactive          |\n");
    printf("+-------------------------------------------------------+\n");
    
    TestBasicInsertionSearch();
    TestAVLBalancing();
    TestDeletion();
    TestTraversalAndStatistik();
    TestSinonimHandling();
    
    printf("\n");
    printf("+-------------------------------------------------------+\n");
    printf("|               ALL TESTS COMPLETED                     |\n");
    printf("+-------------------------------------------------------+\n");
    printf("\n");
    
    return 0;
}
