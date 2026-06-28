/* File    : gui.h                                          */
/* Desk    : Deklarasi Fungsi-fungsi untuk User Interface   */
/* Oleh    : Muhammad Faqih Shiam (251524023)              */
/*           Rajbi Muhammad Nizar (251524027)              */
/* Tgl     : 2024                                           */
/* Mk      : Struktur Data dan Algoritma                   */

#ifndef GUI_H
#define GUI_H

#include "trie.h"
#include "utility.h"

/* ============================================================
   LAYOUT
   Row 0-2 : Header
   Row 3   : Input line  "  >> ..."
   Row 4   : (kosong)
   Row 5   : [Terjemahan]
   Row 6   : [Sinonim   ]
   Row 7   : [Koreksi   ]
   Row 8   : [Saran     ]
   Row 9   : (kosong)
   Row 10  : Hint keys
   ============================================================ */
#define ROW_HINT    1
#define ROW_INPUT   3
#define ROW_TRANS   5
#define ROW_SINONIM 6
#define ROW_KOREKSI 7
#define ROW_SARAN   8
#define PREFIX_LEN  4   /* "  > " */

void DrawHeader();

void DrawHint();

void RedrawAll(char *sentence, char *word,
               char *last_word, char *translation,
               char **thes, int show_thes,
               Trie kamus_trie, Candidate *top3);

#endif //GUI_H
