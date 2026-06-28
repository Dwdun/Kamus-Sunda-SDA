/* File    : gui.c                                          */
/* Desk    : Implementasi Fungsi-fungsi untuk User Interface*/
/* Oleh    : Muhammad Faqih Shiam (251524023)              */
/*           Rajbi Muhammad Nizar (251524027)              */
/* Tgl     : 2024                                           */
/* Mk      : Struktur Data dan Algoritma                   */

#include <stdio.h>
#include <string.h>
#include "gui.h"
#include "utility.h"

void DrawHeader() {
    system("cls");
    printf("=== NYUNDA - Kamus Sunda Interaktif ===\n");
}

void DrawHint() {
    GotoXY(0, ROW_HINT);
    printf("Petunjuk: SPACE = terjemahkan | TAB = autocomplete | ? = sinonim | ESC = keluar");
}

void RedrawAll(char *sentence, char *word,
               char *last_word, char *translation,
               char **thes, int show_thes,
               Trie kamus_trie, Candidate *top3) {

    int i;

    /* --- Baris input --- */
    ClearLineAt(ROW_INPUT);
    printf("  > %s%s", sentence, word);

    /* --- Terjemahan --- */
    ClearLineAt(ROW_TRANS);
    GotoXY(0, ROW_TRANS);
    if (last_word && strlen(last_word) > 0) {
        if (translation) {
            printf("  [Terjemahan] : %s -> %s", last_word, translation);
        } else {
            printf("  [Terjemahan] : '%s' tidak ditemukan", last_word);
        }
    }

    /* --- Sinonim --- */
    ClearLineAt(ROW_SINONIM);
    GotoXY(0, ROW_SINONIM);
    if (show_thes && thes) {
        printf("  [Sinonim] : ");
        int any = 0;
        for (i = 0; i < MaxSinonim; i++) {
            if (thes[i] != Nil && strlen(thes[i]) > 0) {
                if (any) printf(", ");
                printf("%s", thes[i]);
                any = 1;
            }
        }
        if (!any) {
            printf("(tidak ada)");
        }
    }

    /* --- Koreksi top-3 --- */
    ClearLineAt(ROW_KOREKSI);
    GotoXY(0, ROW_KOREKSI);
    if (last_word && strlen(last_word) > 0 && translation == Nil) {
        int any = 0;
        for (i = 0; i < TOP_N; i++) {
            if (top3[i].word != Nil) { any = 1; break; }
        }
        if (any) {
            printf("  [Maksud Anda] : ");
            int first = 1;
            for (i = 0; i < TOP_N; i++) {
                if (top3[i].word != Nil) {
                    if (!first) printf(", ");
                    printf("%s", top3[i].word);
                    first = 0;
                }
            }
        }
    }

    /* --- Saran auto-suggest (Trie) --- */
    ClearLineAt(ROW_SARAN);
    GotoXY(0, ROW_SARAN);
    if (strlen(word) >= 1) {
        printf("  [Saran] : ");
        char *suggestions[3] = {Nil, Nil, Nil};
        int sugg_count = 0;
        GetSuggestions(kamus_trie, word, suggestions, &sugg_count, 3);
        if (sugg_count > 0) {
            int first = 1;
            for (i = 0; i < sugg_count; i++) {
                if (!first) printf(", ");
                printf("%s", suggestions[i]);
                first = 0;
            }
        } else {
            printf("(tidak ada)");
        }
    }

    /* --- Kembali ke cursor input --- */
    int cur_col = PREFIX_LEN + (int)strlen(sentence) + (int)strlen(word);
    GotoXY(cur_col, ROW_INPUT);
}
