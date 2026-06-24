/* File    : main.c                                         */
/* Desk    : Program Utama Kamus Sunda Interaktif (Nyunda)  */
/* Oleh    : Muhammad Faqih Shiam (251524023)              */
/*           Rajbi Muhammad Nizar (251524027)              */
/* Tgl     : 2024                                           */
/* Mk      : Struktur Data dan Algoritma                   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include "utility.h"
#include "bst_avl.h"
#include "trie.h"
#include "stack.h"

#define DATA_FILE     "data/data_kamus_sunda.csv"
#define DATA_FILE_ALT "../data/data_kamus_sunda.csv"
#define MAX_WORD      128
#define MAX_SENTENCE  1024
#define TOP_N         3

/* ============================================================
   WARNA CONSOLE
   ============================================================ */
#define CLR_NORMAL  (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)
#define CLR_WHITE   (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define CLR_GREEN   (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define CLR_YELLOW  (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define CLR_CYAN    (FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define CLR_RED     (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define CLR_GRAY    (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

static HANDLE hConsole;
static Trie kamus_trie;

/* ============================================================
   CONSOLE HELPERS
   ============================================================ */

void SetClr(int color) {
    SetConsoleTextAttribute(hConsole, color);
}

void GotoXY(int x, int y) {
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hConsole, pos);
}

int GetConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hConsole, &info);
    return info.dwSize.X;
}

void ClearLineAt(int y) {
    int w = GetConsoleWidth();
    int i;
    GotoXY(0, y);
    for (i = 0; i < w - 1; i++) putchar(' ');
    GotoXY(0, y);
}

/* ============================================================
   TOP-3 AUTO-CORRECT HELPER
   ============================================================ */

typedef struct {
    char *word;
    int   dist;
} Candidate;

static Candidate top3[TOP_N];

static void dfs_top3(AVLTree node, char *input) {
    int i, worst_idx;
    if (node == Nil) return;

    int d = LevenshteinDistance(input, node->key);

    /* Cari slot dengan dist paling besar */
    worst_idx = 0;
    for (i = 1; i < TOP_N; i++) {
        if (top3[i].dist > top3[worst_idx].dist) worst_idx = i;
    }

    /* Ganti jika lebih baik dan dalam batas */
    if (d <= MaxLevenshteinDist && d < top3[worst_idx].dist) {
        top3[worst_idx].word = node->key;
        top3[worst_idx].dist = d;
    }

    dfs_top3(node->left, input);
    dfs_top3(node->right, input);
}

void ResetTop3() {
    int i;
    for (i = 0; i < TOP_N; i++) {
        top3[i].word = Nil;
        top3[i].dist = MaxLevenshteinDist + 1;
    }
}

void FindTop3(AVLTree tree, char *input) {
    ResetTop3();
    dfs_top3(tree, input);
    /* Sort ascending by dist */
    int i, j;
    for (i = 0; i < TOP_N - 1; i++) {
        for (j = i + 1; j < TOP_N; j++) {
            if (top3[j].dist < top3[i].dist) {
                Candidate tmp = top3[i];
                top3[i] = top3[j];
                top3[j] = tmp;
            }
        }
    }
}

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
#define ROW_INPUT   3
#define ROW_TRANS   5
#define ROW_SINONIM 6
#define ROW_KOREKSI 7
#define ROW_SARAN   8
#define ROW_HINT    10
#define PREFIX_LEN  4   /* "  > " */

void DrawHeader() {
    system("cls");
    SetClr(CLR_CYAN);
    printf("+----------------------------------------------------------+\n");
    printf("|         NYUNDA  -  Kamus Sunda Interaktif                |\n");
    printf("+----------------------------------------------------------+\n");
    SetClr(CLR_NORMAL);
}

void DrawHint() {
    GotoXY(0, ROW_HINT);
    SetClr(CLR_GRAY);
    printf("  SPACE=terjemahan  ?=sinonim  TAB=autocomplete  ESC=keluar");
    SetClr(CLR_NORMAL);
}

/* ============================================================
   REDRAW SELURUH AREA STATUS
   ============================================================ */

void RedrawAll(char *sentence, char *word,
               char *last_word, char *translation,
               char **thes, int show_thes) {

    int i;

    /* --- Baris input --- */
    ClearLineAt(ROW_INPUT);
    SetClr(CLR_GRAY);
    printf("  > ");
    SetClr(CLR_NORMAL);
    printf("%s", sentence);
    SetClr(CLR_GREEN);
    printf("%s", word);
    SetClr(CLR_NORMAL);

    /* --- Terjemahan --- */
    ClearLineAt(ROW_TRANS);
    GotoXY(0, ROW_TRANS);
    if (last_word && strlen(last_word) > 0) {
        if (translation) {
            SetClr(CLR_YELLOW);
            printf("  [Terjemahan] ");
            SetClr(CLR_WHITE);
            printf("%s", last_word);
            SetClr(CLR_NORMAL);
            printf("  ->  ");
            SetClr(CLR_GREEN);
            printf("%s", translation);
        } else {
            SetClr(CLR_RED);
            printf("  [Terjemahan] '%s' tidak ditemukan dalam kamus.", last_word);
        }
        SetClr(CLR_NORMAL);
    }

    /* --- Sinonim --- */
    ClearLineAt(ROW_SINONIM);
    GotoXY(0, ROW_SINONIM);
    if (show_thes && thes) {
        SetClr(CLR_CYAN);
        printf("  [Sinonim   ] ");
        SetClr(CLR_NORMAL);
        int any = 0;
        for (i = 0; i < MaxSinonim; i++) {
            if (thes[i] != Nil && strlen(thes[i]) > 0) {
                if (any) { SetClr(CLR_GRAY); printf("  |  "); }
                SetClr(CLR_WHITE);
                printf("%s", thes[i]);
                SetClr(CLR_NORMAL);
                any = 1;
            }
        }
        if (!any) {
            SetClr(CLR_GRAY);
            printf("(tidak ada sinonim)");
        }
        SetClr(CLR_NORMAL);
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
            SetClr(CLR_YELLOW);
            printf("  [Koreksi   ] Maksud kamu: ");
            SetClr(CLR_NORMAL);
            int first = 1;
            for (i = 0; i < TOP_N; i++) {
                if (top3[i].word != Nil) {
                    if (!first) { SetClr(CLR_GRAY); printf("  |  "); }
                    SetClr(CLR_WHITE);
                    printf("%s", top3[i].word);
                    SetClr(CLR_NORMAL);
                    first = 0;
                }
            }
        }
    }

    /* --- Saran auto-suggest (Trie) --- */
    ClearLineAt(ROW_SARAN);
    GotoXY(0, ROW_SARAN);
    if (strlen(word) >= 1) {
        SetClr(CLR_CYAN);
        printf("  [Saran     ] ");
        
        char *suggestions[3] = {Nil, Nil, Nil};
        int sugg_count = 0;
        GetSuggestions(kamus_trie, word, suggestions, &sugg_count, 3);
        if (sugg_count > 0) {
            int first = 1;
            for (i = 0; i < sugg_count; i++) {
                if (!first) { SetClr(CLR_GRAY); printf("  |  "); }
                SetClr(CLR_WHITE);
                printf("%s", suggestions[i]);
                first = 0;
            }
        } else {
            SetClr(CLR_GRAY);
            printf("(tidak ada saran)");
        }
        SetClr(CLR_NORMAL);
    }

    /* --- Kembali ke cursor input --- */
    int cur_col = PREFIX_LEN + (int)strlen(sentence) + (int)strlen(word);
    GotoXY(cur_col, ROW_INPUT);
}

/* ============================================================
   MAIN
   ============================================================ */

int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    /* Load kamus */
    DrawHeader();
    GotoXY(0, ROW_INPUT);
    SetClr(CLR_YELLOW);
    printf("  Memuat kamus, harap tunggu...");
    SetClr(CLR_NORMAL);

    AVLTree kamus;
    CreateEmptyBST(&kamus);

    boolean loaded = LoadKamusSunda(&kamus, DATA_FILE);
    if (!loaded) loaded = LoadKamusSunda(&kamus, DATA_FILE_ALT);

    CreateEmptyTrie(&kamus_trie);
    if (loaded) {
        BuildTrieFromAVL(kamus_trie, kamus);
    }

    if (!loaded) {
        SetClr(CLR_RED);
        GotoXY(0, ROW_INPUT);
        printf("  [ERROR] Gagal memuat data kamus! Pastikan file CSV tersedia.\n");
        SetClr(CLR_NORMAL);
        return 1;
    }

    DrawHeader();
    DrawHint();
    ResetTop3();

    /* State */
    char  sentence[MAX_SENTENCE] = "";
    char  word[MAX_WORD]         = "";
    char  last_word[MAX_WORD]    = "";
    char *translation            = Nil;
    char **thes                  = Nil;
    int   show_thes              = 0;

    Stack undo_stack;
    Stack redo_stack;
    CreateEmptyStack(&undo_stack);
    CreateEmptyStack(&redo_stack);

    RedrawAll(sentence, word, last_word, translation, thes, show_thes);

    /* Flush sisa input dari terminal (misal Enter yang terbaca dari command) */
    while (_kbhit()) _getch();

    int running = 1;
    while (running) {
        int ch = _getch();

        /* Special key (arrow keys, F-keys) */
        if (ch == 0 || ch == 0xE0) {
            int ch2 = _getch();
            int cur_col = PREFIX_LEN + (int)strlen(sentence) + (int)strlen(word);

            if (ch2 == 0x4B) {  /* Arrow Left = Undo */
                if (!IsEmptyStack(undo_stack)) {
                    char *undone = PopStr(&undo_stack);
                    Push(&redo_stack, undone);
                    free(undone);
                    ReconstructSentence(undo_stack, sentence);
                    
                    /* Update last_word dan status ke top of undo_stack */
                    if (!IsEmptyStack(undo_stack)) {
                        strcpy(last_word, undo_stack.top->word);
                        addressAVL found = SearchBST(kamus, last_word);
                        if (found != Nil) {
                            translation = found->terjemahan_id;
                            ResetTop3();
                        } else {
                            translation = Nil;
                            FindTop3(kamus, last_word);
                        }
                    } else {
                        last_word[0] = '\0';
                        translation = Nil;
                        ResetTop3();
                    }
                    if (thes) { FreeStrArray(thes); thes = Nil; }
                    show_thes = 0;
                    RedrawAll(sentence, word, last_word, translation, thes, show_thes);
                }
            } else if (ch2 == 0x4D) {  /* Arrow Right = Redo */
                if (!IsEmptyStack(redo_stack)) {
                    char *redone = PopStr(&redo_stack);
                    Push(&undo_stack, redone);
                    free(redone);
                    ReconstructSentence(undo_stack, sentence);
                    
                    /* Update last_word dan status ke top of undo_stack */
                    strcpy(last_word, undo_stack.top->word);
                    addressAVL found = SearchBST(kamus, last_word);
                    if (found != Nil) {
                        translation = found->terjemahan_id;
                        ResetTop3();
                    } else {
                        translation = Nil;
                        FindTop3(kamus, last_word);
                    }
                    if (thes) { FreeStrArray(thes); thes = Nil; }
                    show_thes = 0;
                    RedrawAll(sentence, word, last_word, translation, thes, show_thes);
                }
            }
            continue;
        }

        /* ESC = keluar */
        if (ch == 27) { running = 0; break; }

        /* BACKSPACE */
        if (ch == 8) {
            int wlen = (int)strlen(word);
            if (wlen > 0) {
                word[wlen - 1] = '\0';
            } else {
                /* Hapus kata terakhir dari sentence */
                int slen = (int)strlen(sentence);
                if (slen > 0) {
                    sentence[slen - 1] = '\0'; /* hapus spasi */
                    char *sp = strrchr(sentence, ' ');
                    if (sp) {
                        strcpy(word, sp + 1);
                        *(sp + 1) = '\0';
                    } else {
                        strcpy(word, sentence);
                        sentence[0] = '\0';
                    }
                    
                    /* Pop kata terakhir dari undo_stack karena kembali diedit */
                    if (!IsEmptyStack(undo_stack)) {
                        char *popped = PopStr(&undo_stack);
                        free(popped);
                    }
                }
            }
            show_thes = 0;
            RedrawAll(sentence, word, last_word, translation, thes, show_thes);
            continue;
        }

        /* TAB = autocomplete */
        if (ch == 9) {
            if (strlen(word) >= 1) {
                char *suggestions[1] = {Nil};
                int count = 0;
                GetSuggestions(kamus_trie, word, suggestions, &count, 1);
                if (count > 0) {
                    strcpy(word, suggestions[0]);
                    show_thes = 0;
                    RedrawAll(sentence, word, last_word, translation, thes, show_thes);
                }
            }
            continue;
        }

        /* ENTER = proses seperti SPACE lalu stop */
        if (ch == 13) {
            if (strlen(word) == 0 && strlen(sentence) == 0) { running = 0; break; }
            ch = ' ';
        }

        /* SPACE = proses kata saat ini */
        if (ch == ' ') {
            if (strlen(word) == 0) continue;

            Push(&undo_stack, word);
            DestroyStack(&redo_stack);
            CreateEmptyStack(&redo_stack);

            strcpy(last_word, word);
            strcat(sentence, word);
            strcat(sentence, " ");
            word[0] = '\0';

            if (thes) { FreeStrArray(thes); thes = Nil; }
            show_thes = 0;

            addressAVL found = SearchBST(kamus, last_word);
            if (found != Nil) {
                translation = found->terjemahan_id;
                ResetTop3();
            } else {
                translation = Nil;
                FindTop3(kamus, last_word);
            }

            RedrawAll(sentence, word, last_word, translation, thes, show_thes);
            continue;
        }

        /* ? = tampilkan sinonim */
        if (ch == '?') {
            char *target = (strlen(word) > 0) ? word : last_word;
            if (strlen(target) > 0) {
                if (thes) FreeStrArray(thes);
                thes = GetThesaurus(kamus, target);
                show_thes = 1;
            }
            RedrawAll(sentence, word, last_word, translation, thes, show_thes);
            continue;
        }

        /* Karakter biasa */
        if (ch >= 32 && ch <= 126 && (int)strlen(word) < MAX_WORD - 1) {
            int wlen = (int)strlen(word);
            word[wlen]     = (char)ch;
            word[wlen + 1] = '\0';
            show_thes = 0;
        }

        RedrawAll(sentence, word, last_word, translation, thes, show_thes);
    }

    /* Cleanup */
    if (thes) FreeStrArray(thes);
    DestroyBST(&kamus);
    DestroyTrie(&kamus_trie);
    DestroyStack(&undo_stack);
    DestroyStack(&redo_stack);

    GotoXY(0, ROW_HINT + 2);
    SetClr(CLR_CYAN);
    printf("\n  Sampai jumpa! Hatur nuhun parantos nganggo Nyunda.\n\n");
    SetClr(CLR_NORMAL);

    return 0;
}
