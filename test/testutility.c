/* File    : testutility.c                                  */
/* Desk    : Test Unit untuk Utility Functions (utility.h/c) */
/* Oleh    : Muhammad Faqih Shiam (251524023)              */
/*           Rajbi Muhammad Nizar (251524027)              */
/* Tgl     : 2024                                           */
/* Mk      : Struktur Data dan Algoritma                   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/utility.h"
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
   TEST 3: LEVENSHTEIN DISTANCE
   ============================================================ */

void TestLevenshteinDistance() {
    PrintTestCase(3, "Levenshtein Distance");
    
    /* Test Case 3.1: Identical strings */
    printf("\n3.1. Identical strings\n");
    int dist = LevenshteinDistance("bagja", "bagja");
    printf("  Distance('bagja', 'bagja'): %d (expected: 0)\n", dist);
    PrintResult(dist == 0);
    
    /* Test Case 3.2: One character difference */
    printf("\n3.2. One character difference (substitution)\n");
    dist = LevenshteinDistance("bagja", "bagji");
    printf("  Distance('bagja', 'bagji'): %d (expected: 1)\n", dist);
    PrintResult(dist == 1);
    
    /* Test Case 3.3: One character deletion */
    printf("\n3.3. One character deletion\n");
    dist = LevenshteinDistance("bagja", "baja");
    printf("  Distance('bagja', 'baja'): %d (expected: 1)\n", dist);
    PrintResult(dist == 1);
    
    /* Test Case 3.4: One character insertion */
    printf("\n3.4. One character insertion\n");
    dist = LevenshteinDistance("baja", "bagja");
    printf("  Distance('baja', 'bagja'): %d (expected: 1)\n", dist);
    PrintResult(dist == 1);
    
    /* Test Case 3.5: Multiple differences */
    printf("\n3.5. Multiple differences (3 substitutions)\n");
    dist = LevenshteinDistance("abc", "def");
    printf("  Distance('abc', 'def'): %d (expected: 3)\n", dist);
    PrintResult(dist == 3);
    
    /* Test Case 3.6: Different lengths */
    printf("\n3.6. Different lengths\n");
    dist = LevenshteinDistance("lemes", "lema");
    printf("  Distance('lemes', 'lema'): %d\n", dist);
    PrintResult(dist > 0);
    
    /* Test Case 3.7: Empty strings */
    printf("\n3.7. Empty string handling\n");
    dist = LevenshteinDistance("", "abc");
    printf("  Distance('', 'abc'): %d\n", dist);
    dist = LevenshteinDistance("abc", "");
    printf("  Distance('abc', ''): %d\n", dist);
    PrintResult(true);  /* Just ensure no crash */
}

/* ============================================================
   TEST 4: AUTO-CORRECT TYPO
   ============================================================ */

void TestAutoCorrectTypo() {
    PrintTestCase(4, "Auto-Correct Typo");
    
    /* Setup: Build AVL tree dengan beberapa kata */
    printf("\n4.1. Setup: Build AVL-BST with words\n");
    AVLTree tree;
    CreateEmptyBST(&tree);
    
    char *empty_syn[] = {"", "", ""};
    tree = InsertBST(tree, "bagja", "senang", empty_syn);
    tree = InsertBST(tree, "lemes", "halus", empty_syn);
    tree = InsertBST(tree, "ageng", "besar", empty_syn);
    tree = InsertBST(tree, "baé", "ayah", empty_syn);
    
    printf("  Inserted: bagja, lemes, ageng, baé\n");
    
    /* Test Case 4.2: Correct typo dengan distance 1 */
    printf("\n4.2. Correct typo dengan distance 1: 'bagji'\n");
    char *result = AutoCorrectTypo(tree, "bagji");
    printf("  Suggestion: %s (expected: bagja)\n", result ? result : "Nil");
    PrintResult(result != Nil && strcmp(result, "bagja") == 0);
    
    /* Test Case 4.3: Correct typo dengan distance 1: 'agene' */
    printf("\n4.3. Correct typo dengan distance 1: 'agene'\n");
    result = AutoCorrectTypo(tree, "agene");
    printf("  Suggestion: %s (expected: ageng or similar)\n", result ? result : "Nil");
    PrintResult(result != Nil);
    
    /* Test Case 4.4: Exact match */
    printf("\n4.4. Exact match: 'lemes'\n");
    result = AutoCorrectTypo(tree, "lemes");
    printf("  Suggestion: %s (expected: lemes)\n", result ? result : "Nil");
    PrintResult(result != Nil && strcmp(result, "lemes") == 0);
    
    /* Test Case 4.5: No correction (distance > MaxLevenshteinDist) */
    printf("\n4.5. No correction (distance > %d): 'xyz'\n", MaxLevenshteinDist);
    result = AutoCorrectTypo(tree, "xyz");
    printf("  Suggestion: %s (expected: Nil)\n", result ? result : "Nil");
    PrintResult(result == Nil);
    
    /* Test Case 4.6: Multiple typos */
    printf("\n4.6. Multiple typos: 'bgja' (distance 1)\n");
    result = AutoCorrectTypo(tree, "bgja");
    printf("  Suggestion: %s\n", result ? result : "Nil");
    PrintResult(result != Nil);
    
    /* Cleanup */
    DestroyBST(&tree);
}

/* ============================================================
   TEST 5: THESAURUS / SINONIM
   ============================================================ */

void TestThesaurus() {
    PrintTestCase(5, "Thesaurus / Sinonim");
    
    /* Setup: Build AVL tree dengan sinonim */
    printf("\n5.1. Setup: Build AVL-BST with synonyms\n");
    AVLTree tree;
    CreateEmptyBST(&tree);
    
    char *syn_bagja[] = {"mulia", "gemilang", "bahagia"};
    tree = InsertBST(tree, "bagja", "senang", syn_bagja);
    
    char *syn_lemes[] = {"halus", "sopan", "santun"};
    tree = InsertBST(tree, "lemes", "halus", syn_lemes);
    
    char *syn_ageng[] = {"besar", "luas", ""};
    tree = InsertBST(tree, "ageng", "besar", syn_ageng);
    
    printf("  Inserted with synonyms\n");
    
    /* Test Case 5.2: GetThesaurus existing word */
    printf("\n5.2. GetThesaurus for 'bagja'\n");
    char **thes = GetThesaurus(tree, "bagja");
    printf("  Synonyms:\n");
    int i;
    for (i = 0; i < MaxSinonim && thes[i] != Nil; i++) {
        printf("    [%d] %s\n", i+1, thes[i]);
    }
    PrintResult(thes[0] != Nil && strcmp(thes[0], "mulia") == 0);
    FreeStrArray(thes);
    
    /* Test Case 5.3: GetThesaurus with partial sinonim */
    printf("\n5.3. GetThesaurus for 'ageng' (partial synonyms)\n");
    thes = GetThesaurus(tree, "ageng");
    printf("  Synonyms:\n");
    for (i = 0; i < MaxSinonim; i++) {
        if (thes[i] != Nil) {
            printf("    [%d] %s\n", i+1, thes[i]);
        } else {
            printf("    [%d] (empty)\n", i+1);
        }
    }
    PrintResult(thes[0] != Nil && thes[2] == Nil);
    FreeStrArray(thes);
    
    /* Test Case 5.4: GetThesaurus non-existing word */
    printf("\n5.4. GetThesaurus for non-existing word 'xyz'\n");
    thes = GetThesaurus(tree, "xyz");
    printf("  First element is Nil: %s\n", (thes[0] == Nil) ? "true" : "false");
    PrintResult(thes[0] == Nil);
    FreeStrArray(thes);
    
    /* Cleanup */
    DestroyBST(&tree);
}



/* ============================================================
   TEST 6: LOAD KAMUS DARI CSV
   ============================================================ */

void TestLoadKamus() {
    PrintTestCase(6, "Load Kamus Dari CSV");
    
    AVLTree tree;
    CreateEmptyBST(&tree);
    
    printf("\n6.1. Load dari file data_kamus_sunda.csv\n");
    /* Kita coba dua path berbeda, mengantisipasi dari mana program ini di-run (root atau folder test) */
    boolean loaded = LoadKamusSunda(&tree, "data/data_kamus_sunda.csv");
    if (!loaded) {
        loaded = LoadKamusSunda(&tree, "../data/data_kamus_sunda.csv");
    }
    
    printf("  Status load: %s\n", loaded ? "BERHASIL" : "GAGAL");
    PrintResult(loaded);
    
    if (loaded) {
        printf("\n6.2. Cek jumlah node di dalam tree\n");
        int count = NbElmtBST(tree);
        printf("  Total kata ter-load: %d\n", count);
        PrintResult(count > 0);
        
        printf("\n6.3. Coba cari kata 'abah'\n");
        addressAVL found = SearchBST(tree, "abah");
        if (found != Nil) {
            printf("  Ditemukan: %s -> %s\n", found->key, found->terjemahan_id);
            printf("  Sinonim: %s, %s, %s\n", 
                found->sinonim[0] != Nil ? found->sinonim[0] : "-",
                found->sinonim[1] != Nil ? found->sinonim[1] : "-",
                found->sinonim[2] != Nil ? found->sinonim[2] : "-");
        } else {
            printf("  Kata 'abah' tidak ditemukan!\n");
        }
        PrintResult(found != Nil);
    }
    
    DestroyBST(&tree);
}

/* ============================================================
   MAIN TEST RUNNER
   ============================================================ */

int main() {
    printf("\n");
    printf("+-------------------------------------------------------+\n");
    printf("|     TEST UNIT: UTILITY FUNCTIONS (utility.h/c)        |\n");
    printf("|    Aplikasi Nyunda - Kamus Sunda Interactive          |\n");
    printf("+-------------------------------------------------------+\n");
    
    TestLevenshteinDistance();
    TestAutoCorrectTypo();
    TestThesaurus();
    TestLoadKamus();
    
    printf("\n");
    printf("+-------------------------------------------------------+\n");
    printf("|               ALL TESTS COMPLETED                     |\n");
    printf("+-------------------------------------------------------+\n");
    printf("\n");
    
    return 0;
}
