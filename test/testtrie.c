/* File    : testtrie.c                                     */
/* Desk    : Unit Test untuk Struktur Data Trie (trie.h/c)  */
/* Oleh    : Antigravity                                    */
/* Tgl     : 2026                                           */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/trie.h"

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

int main() {
    PrintSeparator("TEST UNIT: TRIE (trie.h/c)");

    Trie T;
    CreateEmptyTrie(&T);

    /* Test Case 1: Inisialisasi */
    PrintTestCase(1, "Inisialisasi Trie");
    PrintResult(T != Nil);

    /* Test Case 2: Insertion dan Search */
    PrintTestCase(2, "Insertion & Search Kata (termasuk karakter khusus)");
    InsertTrie(T, "abdi");
    InsertTrie(T, "abong");
    InsertTrie(T, "abot");
    InsertTrie(T, "aduga-adugan"); /* Kata dengan tanda hubung */
    InsertTrie(T, "mu'alip");       /* Kata dengan apostrof */

    boolean s1 = SearchTrie(T, "abdi");
    boolean s2 = SearchTrie(T, "ABONG");           /* Case-insensitive search */
    boolean s3 = SearchTrie(T, "aduga-adugan");
    boolean s4 = SearchTrie(T, "mu'alip");
    boolean s5 = SearchTrie(T, "tidak_ada");

    printf("  Search 'abdi': %s (Expected: true)\n", s1 ? "true" : "false");
    printf("  Search 'ABONG' (uppercase): %s (Expected: true)\n", s2 ? "true" : "false");
    printf("  Search 'aduga-adugan' (special char): %s (Expected: true)\n", s3 ? "true" : "false");
    printf("  Search 'mu'alip' (special char): %s (Expected: true)\n", s4 ? "true" : "false");
    printf("  Search 'tidak_ada': %s (Expected: false)\n", s5 ? "true" : "false");

    PrintResult(s1 && s2 && s3 && s4 && !s5);

    /* Test Case 3: GetSuggestions (Auto-Suggest) */
    PrintTestCase(3, "Auto-Suggestions berdasarkan Prefiks");
    char *suggestions[3] = {Nil, Nil, Nil};
    int count = 0;

    /* Get suggestions for prefix "ab" */
    GetSuggestions(T, "ab", suggestions, &count, 3);
    printf("  Suggestions untuk prefiks 'ab':\n");
    int i;
    for (i = 0; i < count; i++) {
        printf("    [%d] %s\n", i + 1, suggestions[i]);
    }
    boolean ok_ab = (count == 3 && 
                     strcmp(suggestions[0], "abdi") == 0 &&
                     strcmp(suggestions[1], "abong") == 0 &&
                     strcmp(suggestions[2], "abot") == 0);

    /* Get suggestions for prefix "AB" (case-insensitive) */
    char *suggestions_cap[3] = {Nil, Nil, Nil};
    int count_cap = 0;
    GetSuggestions(T, "AB", suggestions_cap, &count_cap, 3);
    printf("  Suggestions untuk prefiks 'AB' (uppercase):\n");
    for (i = 0; i < count_cap; i++) {
        printf("    [%d] %s\n", i + 1, suggestions_cap[i]);
    }
    boolean ok_cap = (count_cap == 3 && 
                      strcmp(suggestions_cap[0], "abdi") == 0 &&
                      strcmp(suggestions_cap[1], "abong") == 0 &&
                      strcmp(suggestions_cap[2], "abot") == 0);

    /* Get suggestions for prefix "mu" */
    char *suggestions_mu[3] = {Nil, Nil, Nil};
    int count_mu = 0;
    GetSuggestions(T, "mu", suggestions_mu, &count_mu, 3);
    printf("  Suggestions untuk prefiks 'mu':\n");
    for (i = 0; i < count_mu; i++) {
        printf("    [%d] %s\n", i + 1, suggestions_mu[i]);
    }
    boolean ok_mu = (count_mu == 1 && strcmp(suggestions_mu[0], "mu'alip") == 0);

    PrintResult(ok_ab && ok_cap && ok_mu);

    /* Test Case 4: Destruksi Trie */
    PrintTestCase(4, "Destruksi Trie (Cleanup)");
    DestroyTrie(&T);
    printf("  Trie setelah Destroy: %s (Expected: Nil)\n", T == Nil ? "Nil" : "Not Nil");
    PrintResult(T == Nil);

    printf("\n+-------------------------------------------------------+\n");
    printf("|             TRIE TESTS COMPLETED                      |\n");
    printf("+-------------------------------------------------------+\n\n");

    return 0;
}
