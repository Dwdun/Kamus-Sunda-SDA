/* File    : utility.c                                      */
/* Desk    : Implementasi Utility Functions untuk Nyunda    */
/* Oleh    : Muhammad Faqih Shiam (251524023)              */
/*           Rajbi Muhammad Nizar (251524027)              */
/* Tgl     : 2026                                           */
/* Mk      : Struktur Data dan Algoritma                   */

#include "utility.h"

static HANDLE hConsole;

/* ============================================================
   CONSOLE HELPERS
   ============================================================ */

void InitConsole() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
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


void FreeStrArray(char **arr) {
    if ((arr) != Nil) {
        free(arr);
    }
}

/* ============================================================
   LEVENSHTEIN DISTANCE
   ============================================================ */

int LevenshteinDistance(char *s1, char *s2) {
    if ((s1) == Nil || (s2) == Nil) return 0;
    
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    
    /* Alokasi DP table */
    int **dp = (int**) malloc((len1 + 1) * sizeof(int*));
    int i, j;
    for (i = 0; i <= len1; i++) {
        dp[i] = (int*) malloc((len2 + 1) * sizeof(int));
    }
    
    /* Initialize base cases */
    for (i = 0; i <= len1; i++) {
        dp[i][0] = i;
    }
    for (j = 0; j <= len2; j++) {
        dp[0][j] = j;
    }
    
    /* Fill DP table */
    for (i = 1; i <= len1; i++) {
        for (j = 1; j <= len2; j++) {
            if (s1[i-1] == s2[j-1]) {
                dp[i][j] = dp[i-1][j-1];
            } else {
                int insert  = dp[i][j-1]   + 1;
                int delete  = dp[i-1][j]   + 1;
                int replace = dp[i-1][j-1] + 1;
                
                dp[i][j] = (insert < delete) ? 
                           ((insert < replace) ? insert : replace) :
                           ((delete < replace) ? delete : replace);
            }
        }
    }
    
    int result = dp[len1][len2];
    
    /* Dealokasi DP table */
    for (i = 0; i <= len1; i++) {
        free(dp[i]);
    }
    free(dp);
    
    return result;
}

/* ============================================================
   AUTO-CORRECT TYPO
   ============================================================ */

static void dfs_top3(AVLTree node, char *input, Candidate *top3) {
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

    dfs_top3(node->left, input, top3);
    dfs_top3(node->right, input, top3);
}

void ResetTop3(Candidate *top3) {
    int i;
    for (i = 0; i < TOP_N; i++) {
        top3[i].word = Nil;
        top3[i].dist = MaxLevenshteinDist + 1;
    }
}

void FindTop3(AVLTree tree, char *input, Candidate *top3) {
    ResetTop3(top3);
    dfs_top3(tree, input, top3);
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
   THESAURUS / SINONIM
   ============================================================ */

char** GetThesaurus(AVLTree avl_tree, char *word_sunda) {
    char **sinonim = (char**) malloc((MaxSinonim + 1) * sizeof(char*));
    if ((sinonim) == Nil) return Nil;
    
    /* Initialize semua sinonim ke Nil */
    int i;
    for (i = 0; i <= MaxSinonim; i++) {
        sinonim[i] = Nil;
    }
    
    if ((word_sunda) == Nil) return sinonim;
    
    /* Search kata di AVL-BST */
    addressAVL found_node = SearchBST(avl_tree, word_sunda);
    
    if ((found_node) == Nil) {
        /* Kata tidak ditemukan */
        return sinonim;
    }
    
    /* Copy sinonim dari node yang ditemukan */
    for (i = 0; i < MaxSinonim; i++) {
        sinonim[i] = Sinonim(found_node)[i];
    }
    
    return sinonim;
}

/* ============================================================
   LOAD KAMUS DARI CSV
   ============================================================ */

static char* trim_whitespace(char *str) {
    if (str == Nil) return Nil;
    
    /* Trim leading space */
    while(*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') {
        str++;
    }
    
    if(*str == 0) return str; /* All spaces */
    
    /* Trim trailing space */
    char *end = str + strlen(str) - 1;
    while(end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        end--;
    }
    
    end[1] = '\0';
    
    return str;
}

static void parse_csv_line(char *line, char **fields, int max_fields) {
    int count = 0;
    char *start = line;
    
    while (*line && count < max_fields) {
        if (*line == ';') {
            *line = '\0';
            fields[count++] = start;
            start = line + 1;
        } else if (*line == '\n' || *line == '\r') {
            *line = '\0';
        }
        line++;
    }
    /* Sisa kolom terakhir */
    if (count < max_fields) {
        fields[count++] = start;
    }
    /* Isi sisanya dengan empty string jika ada yang kurang */
    while (count < max_fields) {
        fields[count++] = "";
    }
}

boolean LoadKamusSunda(AVLTree *avl_tree, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == Nil) {
        printf("Gagal membuka file: %s\n", filename);
        return false;
    }
    
    char line[1024];
    boolean is_header = true;
    int line_num = 0;
    
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        if (is_header) {
            is_header = false;
            continue; /* Skip header (baris pertama) */
        }
        
        char *fields[5];
        parse_csv_line(line, fields, 5);
        
        char *sunda = trim_whitespace(fields[0]);
        char *terjemahan = trim_whitespace(fields[1]);
        char *thes1 = trim_whitespace(fields[2]);
        char *thes2 = trim_whitespace(fields[3]);
        char *thes3 = trim_whitespace(fields[4]);
        
        if (sunda != Nil && strlen(sunda) > 0 && terjemahan != Nil && strlen(terjemahan) > 0) {
            char *sinonim[MaxSinonim] = {"", "", ""};
            
            if (thes1 != Nil && strlen(thes1) > 0) sinonim[0] = thes1;
            if (thes2 != Nil && strlen(thes2) > 0) sinonim[1] = thes2;
            if (thes3 != Nil && strlen(thes3) > 0) sinonim[2] = thes3;
            
            *avl_tree = InsertBST(*avl_tree, sunda, terjemahan, sinonim);
        }
    }
    
    fclose(file);
    return true;
}


