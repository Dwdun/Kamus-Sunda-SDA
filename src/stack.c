/* File    : stack.c                                        */
/* Desk    : Implementasi Struktur Data Stack untuk Fitur   */
/*           Undo dan Redo Kata pada Aplikasi Nyunda        */
/* Oleh    : Antigravity                                    */
/* Tgl     : 2026                                           */

#include <stdlib.h>
#include <string.h>
#include "stack.h"

/* ==================== KONSTRUKTOR ==================== */

void CreateEmptyStack(Stack *S) {
    if (S != Nil) {
        S->top = Nil;
    }
}

/* ==================== DESTRUCTOR / CLEANUP ==================== */

void DestroyStack(Stack *S) {
    if (S == Nil) return;
    while (!IsEmptyStack(*S)) {
        char *w = PopStr(S);
        if (w != Nil) {
            free(w);
        }
    }
}

/* ==================== OPERASI STACK ==================== */

boolean IsEmptyStack(Stack S) {
    return S.top == Nil;
}

void Push(Stack *S, const char *word) {
    if (S == Nil || word == Nil) return;
    
    addressStack P = (addressStack) malloc(sizeof(StackNode));
    if (P != Nil) {
        P->word = (char*) malloc(strlen(word) + 1);
        if (P->word != Nil) {
            strcpy(P->word, word);
            P->next = S->top;
            S->top = P;
        } else {
            free(P);
        }
    }
}

char* PopStr(Stack *S) {
    if (S == Nil || IsEmptyStack(*S)) return Nil;
    
    addressStack P = S->top;
    char *word = P->word;
    S->top = P->next;
    free(P);
    
    return word;
}

/* ==================== UTILITAS REKONSTRUKSI ==================== */

static void ReconstructSentenceRecursive(addressStack node, char *sentence) {
    if (node == Nil) return;
    
    /* Panggil secara rekursif hingga mencapai elemen terbawah stack */
    ReconstructSentenceRecursive(node->next, sentence);
    
    /* Setelah kembali dari rekursi, tambahkan kata dan spasi ke kalimat (urutan FIFO) */
    if (node->word != Nil && strlen(node->word) > 0) {
        strcat(sentence, node->word);
        strcat(sentence, " ");
    }
}

void ReconstructSentence(Stack S, char *sentence) {
    if (sentence == Nil) return;
    sentence[0] = '\0'; /* Kosongkan kalimat terlebih dahulu */
    ReconstructSentenceRecursive(S.top, sentence);
}
