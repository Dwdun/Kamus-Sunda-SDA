/* File    : stack.h                                        */
/* Desk    : Deklarasi Struktur Data Stack untuk Fitur Undo  */
/*           dan Redo Kata pada Aplikasi Nyunda             */
/* Oleh    : Antigravity                                    */
/* Tgl     : 2026                                           */

#ifndef STACK_H
#define STACK_H

#include "boolean.h"

#ifndef Nil
#define Nil NULL
#endif

/* ==================== DEFINISI TIPE ==================== */

typedef struct tStackNode *addressStack;
typedef struct tStackNode {
    char *word;             /* Menyimpan kata hasil alokasi dinamis */
    addressStack next;      /* Pointer ke elemen di bawahnya */
} StackNode;

typedef struct {
    addressStack top;       /* Pointer ke elemen teratas stack */
} Stack;

/* ==================== KONSTRUKTOR ==================== */

void CreateEmptyStack(Stack *S);
/* Membuat stack kosong: S->top = Nil */

/* ==================== DESTRUCTOR / CLEANUP ==================== */

void DestroyStack(Stack *S);
/* Mengosongkan seluruh elemen stack dan mendealokasi memori */

/* ==================== OPERASI STACK ==================== */

boolean IsEmptyStack(Stack S);
/* Mengembalikan true jika stack kosong, false jika tidak */

void Push(Stack *S, const char *word);
/* Menambahkan kata baru ke dalam stack. Kata akan dialokasikan secara dinamis.
   Jika alokasi gagal, tidak dilakukan penambahan. */

char* PopStr(Stack *S);
/* Mengeluarkan kata teratas dari stack. 
   Mengembalikan pointer ke string kata hasil alokasi (pemanggil bertanggung jawab untuk melakukan free).
   Mengembalikan Nil jika stack kosong. */

/* ==================== UTILITAS REKONSTRUKSI ==================== */

void ReconstructSentence(Stack S, char *sentence);
/* Menyusun kembali seluruh kata di dalam stack menjadi satu kalimat terurut.
   Urutan kata adalah dari elemen terbawah (pertama diketik) ke elemen teratas.
   Kalimat hasil rekonstruksi akan diakhiri dengan satu spasi ekstra.
   Asumsi: sentence memiliki ukuran buffer yang cukup besar. */

#endif /* STACK_H */
