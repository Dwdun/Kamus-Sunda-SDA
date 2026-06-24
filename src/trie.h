/* File    : trie.h                                         */
/* Desk    : Deklarasi Struktur Data Trie untuk Fitur       */
/*           Auto-Suggest dan Auto-Complete Kata            */
/* Oleh    : Muhammad Faqih Shiam (251524023)              */
/*           Rajbi Muhammad Nizar (251524027)              */
/* Tgl     : 2026                                           */

#ifndef TRIE_H
#define TRIE_H

#include "boolean.h"
#include "bst_avl.h"

#ifndef Nil
#define Nil NULL
#endif

#define ALPHABET_SIZE 256

/* ==================== DEFINISI TIPE ==================== */

typedef struct tTrieNode *addressTrie;
typedef struct tTrieNode {
    char *word;                         /* Menyimpan kata lengkap jika node ini adalah akhir dari sebuah kata, Nil jika tidak */
    addressTrie children[ALPHABET_SIZE]; /* Pointer ke anak-anak node (berdasarkan karakter ASCII) */
} TrieNode;

typedef addressTrie Trie;

/* ==================== KONSTRUKTOR ==================== */

void CreateEmptyTrie(Trie *T);
/* Membuat Trie kosong: *T = Nil (atau mengalokasikan root kosong) */

/* ==================== DESTRUCTOR / CLEANUP ==================== */

void DestroyTrie(Trie *T);
/* Dealokasi seluruh node di dalam Trie beserta string kata yang disimpan */

/* ==================== OPERASI TRIE ==================== */

addressTrie AlokasiTrie();
/* Mengalokasikan sebuah node Trie baru, menginisialisasi word = Nil dan semua children = Nil */

void InsertTrie(Trie T, const char *word);
/* Menyisipkan kata baru ke dalam Trie. Jalur penyisipan bersifat case-insensitive,
   namun kata yang disimpan pada node akhir tetap mempertahankan case asli. */

boolean SearchTrie(Trie T, const char *word);
/* Mengembalikan true jika kata ditemukan di dalam Trie, false jika tidak.
   Pencarian bersifat case-insensitive. */

/* ==================== UTILITAS AUTO-SUGGEST ==================== */

void BuildTrieFromAVL(Trie T, AVLTree avl);
/* Menyalin secara rekursif seluruh kosakata yang ada di dalam AVLTree ke dalam Trie. */

void GetSuggestions(Trie T, const char *prefix, char *suggestions[], int *count, int max_suggestions);
/* Mengumpulkan hingga max_suggestions kata terdekat secara alfabetis yang diawali oleh prefix.
   Pencarian prefiks bersifat case-insensitive.
   Hasil saran disimpan ke dalam array of string suggestions.
   Saran string menunjuk langsung ke memori yang dikelola Trie (jangan di-free manual). */

#endif /* TRIE_H */
