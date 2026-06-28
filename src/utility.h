/* File    : utility.h                                      */
/* Desk    : Deklarasi Utility Functions (Auto-Suggest, 
             Auto-Correct) untuk Aplikasi Nyunda           */
/* Oleh    : Muhammad Faqih Shiam (251524023)              */
/*           Rajbi Muhammad Nizar (251524027)              */
/* Tgl     : 2026                                          */
/* Mk      : Struktur Data dan Algoritma                   */

#ifndef UTILITY_H
#define UTILITY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "boolean.h"
#include "bst_avl.h"

/* ==================== MACROS & CONSTANTS ==================== */

#define Nil                 NULL
#define MaxLevenshteinDist  3
#define TOP_N               3

/* ============================================================
   CONSOLE HELPERS
   ============================================================ */
void InitConsole();
void GotoXY(int x, int y);
int GetConsoleWidth();
void ClearLineAt(int y);


/* ==================== UTILITY FUNCTIONS ==================== */

void FreeStrArray(char **arr);
/* Dealokasi array of strings.
   Asumsi: array NULL-terminated. Jika elemen di dalamnya perlu di-free,
   harus dilakukan manual sebelum memanggil fungsi ini. */

/* ==================== LEVENSHTEIN DISTANCE ==================== */

int LevenshteinDistance(char *s1, char *s2);
/* Menghitung Levenshtein distance antara dua string.
   Mengembalikan jumlah minimum edit (insert, delete, replace).
   Kompleksitas: O(len1 * len2) */

/* ==================== AUTO-CORRECT TYPO ==================== */

typedef struct {
    char *word;
    int   dist;
} Candidate;

void ResetTop3(Candidate *top3);

void FindTop3(AVLTree tree, char *input, Candidate *top3);

/* ==================== THESAURUS / SINONIM ==================== */

char** GetThesaurus(AVLTree avl_tree, char *word_sunda);
/* Mengambil array sinonim untuk word_sunda dari AVL-BST.
   Return: array of string (max 3 sinonim) yang NULL-terminated.
   Jika tidak ada sinonim atau kata tidak ditemukan: return empty array.
   
   NOTE: Return pointer ke sinonim yang tersimpan di AVL node
           (jangan di-free stringnya), tapi array-nya HARUS di-free dengan FreeStrArray() */

/* ==================== LOAD KAMUS DARI CSV ==================== */

boolean LoadKamusSunda(AVLTree *avl_tree, const char *filename);
/* Membaca file CSV (dengan separator ';') dan memasukkan isinya ke dalam AVLTree.
   Baris pertama (header) akan dilewati.
   Mengembalikan true jika berhasil, false jika gagal. */

#endif /* UTILITY_H */
