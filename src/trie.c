/* File    : trie.c                                         */
/* Desk    : Implementasi Struktur Data Trie untuk Fitur     */
/*           Auto-Suggest dan Auto-Complete Kata            */
/* Oleh    : Antigravity                                    */
/* Tgl     : 2026                                           */

#include <stdlib.h>
#include <string.h>
#include "trie.h"

/* ==================== HELPER FUNCTIONS ==================== */

/* Mengubah karakter ke huruf kecil untuk menjamin case-insensitive pada jalur Trie */
static unsigned char to_lowercase(unsigned char ch) {
    if (ch >= 'A' && ch <= 'Z') {
        return ch - 'A' + 'a';
    }
    return ch;
}

/* Traversal DFS untuk mengumpulkan kata dari node tertentu */
static void collect_suggestions(addressTrie node, char *suggestions[], int *count, int max_suggestions) {
    if (node == Nil || *count >= max_suggestions) return;
    
    /* Jika node ini merepresentasikan akhir kata, tambahkan ke daftar saran */
    if (node->word != Nil) {
        suggestions[*count] = node->word;
        (*count)++;
    }
    
    /* Telusuri anak-anak secara alfabetis (ASCII 0-255) */
    int i;
    for (i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i] != Nil) {
            collect_suggestions(node->children[i], suggestions, count, max_suggestions);
            if (*count >= max_suggestions) return;
        }
    }
}

/* ==================== KONSTRUKTOR ==================== */

void CreateEmptyTrie(Trie *T) {
    if (T != Nil) {
        *T = AlokasiTrie();
    }
}

addressTrie AlokasiTrie() {
    addressTrie P = (addressTrie) malloc(sizeof(TrieNode));
    if (P != Nil) {
        P->word = Nil;
        int i;
        for (i = 0; i < ALPHABET_SIZE; i++) {
            P->children[i] = Nil;
        }
    }
    return P;
}

/* ==================== DESTRUCTOR / CLEANUP ==================== */

void DestroyTrie(Trie *T) {
    if (T == Nil || *T == Nil) return;
    
    int i;
    for (i = 0; i < ALPHABET_SIZE; i++) {
        if ((*T)->children[i] != Nil) {
            DestroyTrie(&((*T)->children[i]));
        }
    }
    
    if ((*T)->word != Nil) {
        free((*T)->word);
    }
    free(*T);
    *T = Nil;
}

/* ==================== OPERASI TRIE ==================== */

void InsertTrie(Trie T, const char *word) {
    if (T == Nil || word == Nil || strlen(word) == 0) return;
    
    addressTrie curr = T;
    int i;
    for (i = 0; word[i] != '\0'; i++) {
        unsigned char ch = to_lowercase((unsigned char)word[i]);
        if (curr->children[ch] == Nil) {
            curr->children[ch] = AlokasiTrie();
        }
        curr = curr->children[ch];
    }
    
    /* Tandai akhir kata dan simpan kata dengan case asli */
    if (curr->word == Nil) {
        curr->word = (char*) malloc(strlen(word) + 1);
        if (curr->word != Nil) {
            strcpy(curr->word, word);
        }
    }
}

boolean SearchTrie(Trie T, const char *word) {
    if (T == Nil || word == Nil || strlen(word) == 0) return false;
    
    addressTrie curr = T;
    int i;
    for (i = 0; word[i] != '\0'; i++) {
        unsigned char ch = to_lowercase((unsigned char)word[i]);
        if (curr->children[ch] == Nil) {
            return false;
        }
        curr = curr->children[ch];
    }
    
    return (curr->word != Nil);
}

/* ==================== UTILITAS AUTO-SUGGEST ==================== */

void BuildTrieFromAVL(Trie T, AVLTree avl) {
    if (T == Nil || avl == Nil) return;
    
    /* Sisipkan kata Sunda dari node AVL saat ini */
    if (avl->word_sunda != Nil && strlen(avl->word_sunda) > 0) {
        InsertTrie(T, avl->word_sunda);
    }
    
    /* Rekursi ke anak kiri dan kanan AVL tree */
    BuildTrieFromAVL(T, avl->left);
    BuildTrieFromAVL(T, avl->right);
}

void GetSuggestions(Trie T, const char *prefix, char *suggestions[], int *count, int max_suggestions) {
    *count = 0;
    if (T == Nil || prefix == Nil || strlen(prefix) == 0) return;
    
    /* Telusuri Trie sesuai dengan prefiks yang diberikan (case-insensitive) */
    addressTrie curr = T;
    int i;
    for (i = 0; prefix[i] != '\0'; i++) {
        unsigned char ch = to_lowercase((unsigned char)prefix[i]);
        if (curr->children[ch] == Nil) {
            return; /* Prefiks tidak ditemukan, tidak ada saran */
        }
        curr = curr->children[ch];
    }
    
    /* Lakukan DFS untuk mengumpulkan saran kata terdekat di bawah node prefiks */
    collect_suggestions(curr, suggestions, count, max_suggestions);
}
