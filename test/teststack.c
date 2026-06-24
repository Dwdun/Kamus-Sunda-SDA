/* File    : teststack.c                                    */
/* Desk    : Unit Test untuk Struktur Data Stack (stack.h/c)*/
/* Oleh    : Antigravity                                    */
/* Tgl     : 2026                                           */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/stack.h"

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
    PrintSeparator("TEST UNIT: STACK (stack.h/c)");

    Stack S;
    CreateEmptyStack(&S);

    /* Test Case 1: Inisialisasi Stack Kosong */
    PrintTestCase(1, "Inisialisasi Stack Kosong");
    printf("  IsEmptyStack(S): %s (Expected: true)\n", IsEmptyStack(S) ? "true" : "false");
    PrintResult(IsEmptyStack(S) == true);

    /* Test Case 2: Operasi Push */
    PrintTestCase(2, "Operasi Push Kata");
    Push(&S, "abdi");
    Push(&S, "abong");
    Push(&S, "abot");
    printf("  IsEmptyStack(S) setelah 3 push: %s (Expected: false)\n", IsEmptyStack(S) ? "true" : "false");
    PrintResult(IsEmptyStack(S) == false);

    /* Test Case 3: Rekonstruksi Kalimat */
    PrintTestCase(3, "Rekonstruksi Kalimat");
    char sentence[100] = "";
    ReconstructSentence(S, sentence);
    printf("  Kalimat hasil rekonstruksi: \"%s\" (Expected: \"abdi abong abot \")\n", sentence);
    PrintResult(strcmp(sentence, "abdi abong abot ") == 0);

    /* Test Case 4: Operasi Pop */
    PrintTestCase(4, "Operasi Pop Kata");
    char *w1 = PopStr(&S);
    printf("  Pop 1: %s (Expected: abot)\n", w1 ? w1 : "Nil");
    boolean ok1 = (w1 != Nil && strcmp(w1, "abot") == 0);
    free(w1);

    char *w2 = PopStr(&S);
    printf("  Pop 2: %s (Expected: abong)\n", w2 ? w2 : "Nil");
    boolean ok2 = (w2 != Nil && strcmp(w2, "abong") == 0);
    free(w2);

    sentence[0] = '\0';
    ReconstructSentence(S, sentence);
    printf("  Kalimat setelah 2 pop: \"%s\" (Expected: \"abdi \")\n", sentence);
    boolean ok_sentence = (strcmp(sentence, "abdi ") == 0);

    char *w3 = PopStr(&S);
    printf("  Pop 3: %s (Expected: abdi)\n", w3 ? w3 : "Nil");
    boolean ok3 = (w3 != Nil && strcmp(w3, "abdi") == 0);
    free(w3);

    printf("  IsEmptyStack(S) setelah semua pop: %s (Expected: true)\n", IsEmptyStack(S) ? "true" : "false");
    PrintResult(ok1 && ok2 && ok3 && ok_sentence && IsEmptyStack(S));

    /* Test Case 5: Underflow Pop */
    PrintTestCase(5, "Underflow Pop (Pop Stack Kosong)");
    char *w4 = PopStr(&S);
    printf("  Pop dari stack kosong: %s (Expected: Nil)\n", w4 ? w4 : "Nil");
    PrintResult(w4 == Nil);

    /* Test Case 6: Cleanup/Destroy Stack */
    PrintTestCase(6, "Destroy Stack");
    Push(&S, "sunda");
    Push(&S, "nyunda");
    DestroyStack(&S);
    printf("  IsEmptyStack(S) setelah Destroy: %s (Expected: true)\n", IsEmptyStack(S) ? "true" : "false");
    PrintResult(IsEmptyStack(S) == true);

    printf("\n+-------------------------------------------------------+\n");
    printf("|             STACK TESTS COMPLETED                     |\n");
    printf("+-------------------------------------------------------+\n\n");

    return 0;
}
