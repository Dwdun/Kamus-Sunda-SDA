/* File    : bst_avl.h                                      */
/* Desk    : Deklarasi AVL-BST untuk Kamus Sunda           */
/* Oleh    : Muhammad Faqih Shiam (251524023)              */
/*           Rajbi Muhammad Nizar (251524027)              */
/* Tgl     : 2024                                           */
/* Mk      : Struktur Data dan Algoritma                   */

#ifndef BST_AVL_H
#define BST_AVL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "boolean.h"

/* ==================== DEFINISI TIPE ==================== */

#define Nil             NULL
#define MaxSinonim      3

/* Accessor Macros untuk AVLNode */
#define Key(P)          ((P)->key)
#define WordSunda(P)    ((P)->word_sunda)
#define Terjemahan(P)   ((P)->terjemahan_id)
#define Sinonim(P)      ((P)->sinonim)
#define Left(P)         ((P)->left)
#define Right(P)        ((P)->right)
#define Height(P)       ((P)->height)

typedef struct tAVLNode *addressAVL;
typedef struct tAVLNode {
    char *key;                  /* Kata Sunda (untuk sorting/pencarian) */
    char *word_sunda;           /* Kata Sunda lengkap */
    char *terjemahan_id;        /* Terjemahan ke Bahasa Indonesia */
    char *sinonim[MaxSinonim];  /* Array 3 sinonim dalam Bahasa Sunda */
    addressAVL left;            /* Pointer ke child kiri */
    addressAVL right;           /* Pointer ke child kanan */
    int height;                 /* Tinggi subtree untuk balancing */
} AVLNode;

typedef addressAVL AVLTree;     /* Alias untuk root tree */

/* ==================== ALOKASI / DEALOKASI ==================== */

addressAVL AlokasiBST(char *word_sunda, char *terjemahan_id);
/* Menghasilkan address hasil alokasi sebuah AVL-Node.
   Key otomatis di-copy dari word_sunda (lowercase).
   Jika berhasil: semua field terisi, height=0.
   Jika gagal: mengembalikan Nil. */

void DealokasiBST(addressAVL *P);
/* Mendealokasi node yang ditunjuk P termasuk string-nya, 
   lalu set *P = Nil */

/* ==================== KONSTRUKTOR ==================== */

void CreateEmptyBST(AVLTree *T);
/* Membuat BST kosong: *T = Nil */

/* ==================== SELEKTOR ==================== */

boolean IsEmptyBST(AVLTree T);
/* Mengembalikan true jika BST kosong (T == Nil) */

char* GetKeyBST(AVLTree T);
/* Mengirimkan key (kata Sunda) dari node BST T */

char* GetTranslationBST(AVLTree T);
/* Mengirimkan terjemahan Bahasa Indonesia dari node BST T */

char** GetSinonimBST(AVLTree T);
/* Mengirimkan pointer ke array sinonim dari node BST T */

AVLTree GetLeftBST(AVLTree T);
/* Mengirimkan anak kiri BST T */

AVLTree GetRightBST(AVLTree T);
/* Mengirimkan anak kanan BST T */

int GetHeightBST(AVLTree T);
/* Mengirimkan tinggi (height) dari node BST T */

/* ==================== HELPER FUNCTIONS ==================== */

int MaxInt(int a, int b);
/* Mengembalikan nilai maksimal antara a dan b */

int GetHeight(AVLTree T);
/* Helper: mengembalikan height node, 0 jika NULL */

int GetBalanceFactor(AVLTree T);
/* Helper: menghitung balance factor (height kiri - height kanan) */

void UpdateHeight(AVLTree T);
/* Helper: update height node berdasarkan anak-anaknya */

/* ==================== OPERASI ROTASI (BALANCING) ==================== */

AVLTree RotateLeft(AVLTree T);
/* Helper: lakukan rotasi kiri untuk balancing AVL tree */

AVLTree RotateRight(AVLTree T);
/* Helper: lakukan rotasi kanan untuk balancing AVL tree */

AVLTree RotateLeftRight(AVLTree T);
/* Helper: lakukan rotasi kiri-kanan (LR case) */

AVLTree RotateRightLeft(AVLTree T);
/* Helper: lakukan rotasi kanan-kiri (RL case) */

/* ==================== SEARCH ==================== */

addressAVL SearchBST(AVLTree T, char *key);
/* Mengembalikan pointer ke node dengan key yang dicari.
   Jika tidak ditemukan: return Nil.
   Pencarian case-sensitive dan menggunakan strcmp. */

boolean SearchExistBST(AVLTree T, char *key);
/* True jika ada node dengan key yang dicari */

/* ==================== INSERT ==================== */

AVLTree InsertBST(AVLTree T, char *word_sunda, 
                   char *terjemahan_id, 
                   char **sinonim);
/* Sisipkan entry baru ke dalam AVL-BST.
   Otomatis melakukan balancing untuk menjaga AVL property.
   Jika key sudah ada: tidak ada perubahan (atau update optional).
   Return root baru setelah insert. */

/* ==================== DELETE ==================== */

addressAVL FindMinBST(AVLTree T);
/* Mengembalikan pointer ke node dengan key minimum (paling kiri) */

AVLTree DeleteMinBST(AVLTree T);
/* Menghapus node dengan key minimum, return root baru */

AVLTree DeleteBST(AVLTree T, char *key);
/* Menghapus node dengan key tertentu dari AVL-BST.
   Otomatis melakukan balancing.
   Jika key tidak ditemukan: tidak ada perubahan.
   Return root baru setelah delete. */

/* ==================== TRAVERSAL ==================== */

void InorderBST(AVLTree T);
/* Memproses semua simpul T secara Inorder: kiri, akar, kanan.
   Menampilkan: key | terjemahan_id */

void PreorderBST(AVLTree T);
/* Memproses semua simpul T secara Preorder: akar, kiri, kanan */

void PostorderBST(AVLTree T);
/* Memproses semua simpul T secara Postorder: kiri, kanan, akar */

void PrintTreeBST(AVLTree T, char tab[]);
/* Menuliskan semua simpul T dengan indentasi tab (visual tree) */

/* ==================== FUNGSI STATISTIK ==================== */

int NbElmtBST(AVLTree T);
/* Mengembalikan banyaknya node dalam BST T */

int NbDaunBST(AVLTree T);
/* Mengembalikan banyaknya daun (leaf) dalam BST T */

int LevelBST(AVLTree T, char *key);
/* Mengembalikan level node dengan key tertentu.
   Root = level 1. Return 0 jika tidak ditemukan. */

/* ==================== VALIDASI BST ==================== */

boolean IsValidBST(AVLTree T);
/* True jika T adalah valid BST (left < parent < right) */

boolean IsValidAVL(AVLTree T);
/* True jika T adalah valid AVL tree (balance factor ∈ [-1,1]) */

/* ==================== CLEANUP ==================== */

void DestroyBST(AVLTree *T);
/* Dealokasi seluruh tree T secara recursive, lalu set *T = Nil */

#endif /* BST_AVL_H */
