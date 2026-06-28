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
#include "utility.h"
#include "gui.h"
#include "bst_avl.h"
#include "trie.h"
#include "stack.h"

#define DATA_FILE     "data/data_kamus_sunda.csv"
#define DATA_FILE_ALT "../data/data_kamus_sunda.csv"
#define MAX_WORD      128
#define MAX_SENTENCE  1024

/* ============================================================
   MAIN
   ============================================================ */

int main() {
    InitConsole();

    /* Load kamus */
    DrawHeader();
    GotoXY(0, ROW_INPUT);
    printf("  Memuat kamus, silakan tunggu...");

    AVLTree kamus;
    CreateEmptyBST(&kamus);

    boolean loaded = LoadKamusSunda(&kamus, DATA_FILE);
    if (!loaded) loaded = LoadKamusSunda(&kamus, DATA_FILE_ALT);

    Trie kamus_trie;
    CreateEmptyTrie(&kamus_trie);
    if (loaded) {
        BuildTrieFromAVL(kamus_trie, kamus);
    }

    if (!loaded) {
        GotoXY(0, ROW_INPUT);
        printf("  Error: Gagal memuat data kamus. Pastikan file CSV tersedia.\n");
        return 1;
    }

    DrawHeader();
    DrawHint();
    
    Candidate top3[TOP_N];
    ResetTop3(top3);

    /* State */
    char  sentence[MAX_SENTENCE] = "";
    char  word[MAX_WORD]         = "";
    char  last_word[MAX_WORD]    = "";
    char  undone_active_word[MAX_WORD] = "";
    char *translation            = Nil;
    char **thes                  = Nil;
    int   show_thes              = 0;

    Stack undo_stack;
    Stack redo_stack;
    CreateEmptyStack(&undo_stack);
    CreateEmptyStack(&redo_stack);

    RedrawAll(sentence, word, last_word, translation, thes, show_thes, kamus_trie, top3);

    /* Flush sisa input dari terminal (misal Enter yang terbaca dari command) */
    while (_kbhit()) _getch();

    int running = 1;
    while (running) {
        int ch = _getch();

        /* Special key (arrow keys, F-keys) */
        if (ch == 0 || ch == 0xE0) {
            int ch2 = _getch();
            
            if (ch2 == 0x4B) {  /* Arrow Left = Undo */
                if (strlen(word) > 0) {
                    /* Undo kata yang sedang aktif diketik */
                    strcpy(undone_active_word, word);
                    word[0] = '\0';
                    show_thes = 0;
                    RedrawAll(sentence, word, last_word, translation, thes, show_thes, kamus_trie, top3);
                } else if (!IsEmptyStack(undo_stack)) {
                    /* Undo kata yang sudah masuk ke stack kalimat */
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
                            ResetTop3(top3);
                        } else {
                            translation = Nil;
                            FindTop3(kamus, last_word, top3);
                        }
                    } else {
                        last_word[0] = '\0';
                        translation = Nil;
                        ResetTop3(top3);
                    }
                    if (thes) { FreeStrArray(thes); thes = Nil; }
                    show_thes = 0;
                    RedrawAll(sentence, word, last_word, translation, thes, show_thes, kamus_trie, top3);
                }
            } else if (ch2 == 0x4D) {  /* Arrow Right = Redo */
                if (!IsEmptyStack(redo_stack)) {
                    /* Redo kata dari stack kalimat */
                    char *redone = PopStr(&redo_stack);
                    Push(&undo_stack, redone);
                    free(redone);
                    ReconstructSentence(undo_stack, sentence);
                    
                    /* Update last_word dan status ke top of undo_stack */
                    strcpy(last_word, undo_stack.top->word);
                    addressAVL found = SearchBST(kamus, last_word);
                    if (found != Nil) {
                        translation = found->terjemahan_id;
                        ResetTop3(top3);
                    } else {
                        translation = Nil;
                        FindTop3(kamus, last_word, top3);
                    }
                    if (thes) { FreeStrArray(thes); thes = Nil; }
                    show_thes = 0;
                    RedrawAll(sentence, word, last_word, translation, thes, show_thes, kamus_trie, top3);
                } else if (strlen(undone_active_word) > 0) {
                    /* Redo kata yang sedang aktif diketik */
                    strcpy(word, undone_active_word);
                    undone_active_word[0] = '\0';
                    show_thes = 0;
                    RedrawAll(sentence, word, last_word, translation, thes, show_thes, kamus_trie, top3);
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
            undone_active_word[0] = '\0'; /* Reset redo untuk kata aktif karena ada pengeditan */
            RedrawAll(sentence, word, last_word, translation, thes, show_thes, kamus_trie, top3);
            continue;
        }

        /* TAB = state-aware autocomplete/correction */
        if (ch == 9) {
            /* State 1: Auto-correct is active, replace with top suggestion */
            if (translation == Nil && strlen(last_word) > 0 && top3[0].word != Nil) {
                char *old_word = PopStr(&undo_stack);
                free(old_word);
                
                Push(&undo_stack, top3[0].word);
                strcpy(last_word, top3[0].word);
                ReconstructSentence(undo_stack, sentence);

                addressAVL found = SearchBST(kamus, last_word);
                if (found != Nil) {
                    translation = found->terjemahan_id;
                }
                /* ResetTop3 happens automatically on the next search/find */
                ResetTop3(top3); 
                show_thes = 0;
                RedrawAll(sentence, word, last_word, translation, thes, show_thes, kamus_trie, top3);
            }
            /* State 2: Default autocomplete for current word */
            else if (strlen(word) >= 1) {
                char *suggestions[1] = {Nil};
                int count = 0;
                GetSuggestions(kamus_trie, word, suggestions, &count, 1);
                if (count > 0) {
                    strcpy(word, suggestions[0]);
                    show_thes = 0;
                    RedrawAll(sentence, word, last_word, translation, thes, show_thes, kamus_trie, top3);
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
            undone_active_word[0] = '\0'; /* Reset redo untuk kata aktif */

            strcpy(last_word, word);
            strcat(sentence, word);
            strcat(sentence, " ");
            word[0] = '\0';

            if (thes) { FreeStrArray(thes); thes = Nil; }
            show_thes = 0;

            addressAVL found = SearchBST(kamus, last_word);
            if (found != Nil) {
                translation = found->terjemahan_id;
                ResetTop3(top3);
            } else {
                translation = Nil;
                FindTop3(kamus, last_word, top3);
            }

            RedrawAll(sentence, word, last_word, translation, thes, show_thes, kamus_trie, top3);
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
            RedrawAll(sentence, word, last_word, translation, thes, show_thes, kamus_trie, top3);
            continue;
        }

        /* Karakter biasa */
        if (ch >= 32 && ch <= 126 && (int)strlen(word) < MAX_WORD - 1) {
            int wlen = (int)strlen(word);
            word[wlen]     = (char)ch;
            word[wlen + 1] = '\0';
            show_thes = 0;
            undone_active_word[0] = '\0'; /* Reset redo untuk kata aktif karena ada karakter baru */
        }

        RedrawAll(sentence, word, last_word, translation, thes, show_thes, kamus_trie, top3);
    }

    /* Cleanup */
    if (thes) FreeStrArray(thes);
    DestroyBST(&kamus);
    DestroyTrie(&kamus_trie);
    DestroyStack(&undo_stack);
    DestroyStack(&redo_stack);

    GotoXY(0, ROW_SARAN + 2);
    printf("\n  Sampai jumpa! Hatur nuhun parantos nganggo Nyunda.\n\n");

    return 0;
}
