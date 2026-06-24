/* File    : bst_avl.c                                      */
/* Desk    : Implementasi AVL-BST untuk Kamus Sunda        */
/* Oleh    : Muhammad Faqih Shiam (251524023)              */
/*           Rajbi Muhammad Nizar (251524027)              */
/* Tgl     : 2026                                           */
/* Mk      : Struktur Data dan Algoritma                   */

#include "bst_avl.h"

/* ============================================================
ALOKASI / DEALOKASI
   ============================================================ */

addressAVL AlokasiBST(char *word_sunda, char *terjemahan_id) {
    addressAVL P = (addressAVL) malloc(sizeof(AVLNode));
    if ((P) != Nil) {
        /* Alokasi dan copy word_sunda sebagai key (lowercase) */
        Key(P)        = (char*) malloc(strlen(word_sunda) + 1);
        WordSunda(P)  = (char*) malloc(strlen(word_sunda) + 1);
        Terjemahan(P) = (char*) malloc(strlen(terjemahan_id) + 1);
        
        if (Key(P) == Nil || WordSunda(P) == Nil || Terjemahan(P) == Nil) {
            free(Key(P));
            free(WordSunda(P));
            free(Terjemahan(P));
            free(P);
            return Nil;
        }
        
        strcpy(WordSunda(P), word_sunda);
        strcpy(Key(P), word_sunda);        /* Key adalah copy dari word_sunda */
        strcpy(Terjemahan(P), terjemahan_id);
        
        /* Inisialisasi sinonim */
        int i;
        for (i = 0; i < MaxSinonim; i++) {
            Sinonim(P)[i] = Nil;
        }
        
        /* Inisialisasi tree pointers dan height */
        Left(P)   = Nil;
        Right(P)  = Nil;
        Height(P) = 0;
    }
    return P;
}

void DealokasiBST(addressAVL *P) {
    if ((*P) != Nil) {
        if (Key(*P) != Nil)        free(Key(*P));
        if (WordSunda(*P) != Nil)  free(WordSunda(*P));
        if (Terjemahan(*P) != Nil) free(Terjemahan(*P));
        
        /* Dealokasi sinonim jika ada */
        int i;
        for (i = 0; i < MaxSinonim; i++) {
            if (Sinonim(*P)[i] != Nil) {
                free(Sinonim(*P)[i]);
            }
        }
        
        free(*P);
        *P = Nil;
    }
}

/* ============================================================
   KONSTRUKTOR
   ============================================================ */

void CreateEmptyBST(AVLTree *T) {
    *T = Nil;
}

/* ============================================================
   SELEKTOR
   ============================================================ */

boolean IsEmptyBST(AVLTree T) {
    return ((T) == Nil);
}

char* GetKeyBST(AVLTree T) {
    return Key(T);
}

char* GetTranslationBST(AVLTree T) {
    return Terjemahan(T);
}

char** GetSinonimBST(AVLTree T) {
    return Sinonim(T);
}

AVLTree GetLeftBST(AVLTree T) {
    return Left(T);
}

AVLTree GetRightBST(AVLTree T) {
    return Right(T);
}

int GetHeightBST(AVLTree T) {
    return Height(T);
}

/* ============================================================
   HELPER FUNCTIONS
   ============================================================ */

int MaxInt(int a, int b) {
    return (a > b) ? a : b;
}

int GetHeight(AVLTree T) {
    if ((T) == Nil) return -1;
    return Height(T);
}

int GetBalanceFactor(AVLTree T) {
    if ((T) == Nil) return 0;
    return GetHeight(Left(T)) - GetHeight(Right(T));
}

void UpdateHeight(AVLTree T) {
    if ((T) != Nil) {
        Height(T) = MaxInt(GetHeight(Left(T)), GetHeight(Right(T))) + 1;
    }
}

/* ============================================================
   OPERASI ROTASI (BALANCING)
   ============================================================ */

AVLTree RotateLeft(AVLTree T) {
    /* Rotasi kiri:
       T                  R
      / \                / \
     A   R      =>      T   C
        / \            / \
       B   C          A   B
    */
    AVLTree R = Right(T);
    Right(T) = Left(R);
    Left(R)  = T;
    
    UpdateHeight(T);
    UpdateHeight(R);
    
    return R;
}

AVLTree RotateRight(AVLTree T) {
    /* Rotasi kanan:
         T                L
        / \              / \
       L   C    =>      A   T
      / \                  / \
     A   B                B   C
    */
    AVLTree L = Left(T);
    Left(T)  = Right(L);
    Right(L) = T;
    
    UpdateHeight(T);
    UpdateHeight(L);
    
    return L;
}

AVLTree RotateLeftRight(AVLTree T) {
    /* LR case: rotasi kiri dulu, kemudian rotasi kanan */
    Left(T) = RotateLeft(Left(T));
    return RotateRight(T);
}

AVLTree RotateRightLeft(AVLTree T) {
    /* RL case: rotasi kanan dulu, kemudian rotasi kiri */
    Right(T) = RotateRight(Right(T));
    return RotateLeft(T);
}

/* ============================================================
   SEARCH
   ============================================================ */

addressAVL SearchBST(AVLTree T, char *key) {
    if ((T) == Nil) return Nil;
    
    int cmp = strcmp(key, Key(T));
    
    if (cmp == 0) {
        return T;
    } else if (cmp < 0) {
        return SearchBST(Left(T), key);
    } else {
        return SearchBST(Right(T), key);
    }
}

boolean SearchExistBST(AVLTree T, char *key) {
    return (SearchBST(T, key) != Nil);
}

/* ============================================================
   INSERT
   ============================================================ */

AVLTree InsertBST(AVLTree T, char *word_sunda, 
                   char *terjemahan_id, 
                   char **sinonim) {
    if ((T) == Nil) {
        /* Base case: leaf position found */
        T = AlokasiBST(word_sunda, terjemahan_id);
        
        /* Assign sinonim jika ada */
        if ((sinonim) != Nil && (T) != Nil) {
            int i;
            for (i = 0; i < MaxSinonim; i++) {
                if ((sinonim[i]) != Nil && strlen(sinonim[i]) > 0) {
                    Sinonim(T)[i] = (char*) malloc(strlen(sinonim[i]) + 1);
                    if (Sinonim(T)[i] != Nil) {
                        strcpy(Sinonim(T)[i], sinonim[i]);
                    }
                }
            }
        }
        return T;
    }
    
    int cmp = strcmp(word_sunda, Key(T));
    
    if (cmp < 0) {
        Left(T) = InsertBST(Left(T), word_sunda, terjemahan_id, sinonim);
    } else if (cmp > 0) {
        Right(T) = InsertBST(Right(T), word_sunda, terjemahan_id, sinonim);
    }
    /* Jika cmp == 0, key sudah ada: tidak ada perubahan (atau bisa update) */
    
    /* Update height setelah insert */
    UpdateHeight(T);
    
    /* Cek balance factor dan lakukan rotasi jika diperlukan */
    int bf = GetBalanceFactor(T);
    
    /* Left-Left case */
    if (bf > 1 && GetBalanceFactor(Left(T)) >= 0) {
        return RotateRight(T);
    }
    
    /* Left-Right case */
    if (bf > 1 && GetBalanceFactor(Left(T)) < 0) {
        return RotateLeftRight(T);
    }
    
    /* Right-Right case */
    if (bf < -1 && GetBalanceFactor(Right(T)) <= 0) {
        return RotateLeft(T);
    }
    
    /* Right-Left case */
    if (bf < -1 && GetBalanceFactor(Right(T)) > 0) {
        return RotateRightLeft(T);
    }
    
    return T;
}

/* ============================================================
   DELETE
   ============================================================ */

addressAVL FindMinBST(AVLTree T) {
    if ((T) == Nil) return Nil;
    if (Left(T) == Nil) return T;
    return FindMinBST(Left(T));
}

AVLTree DeleteMinBST(AVLTree T) {
    if ((T) == Nil) return Nil;
    
    if (Left(T) == Nil) {
        AVLTree tmp = Right(T);
        DealokasiBST(&T);
        return tmp;
    }
    
    Left(T) = DeleteMinBST(Left(T));
    UpdateHeight(T);
    
    int bf = GetBalanceFactor(T);
    if (bf < -1 && GetBalanceFactor(Right(T)) > 0) {
        return RotateRightLeft(T);
    }
    if (bf < -1) {
        return RotateLeft(T);
    }
    
    return T;
}

AVLTree DeleteBST(AVLTree T, char *key) {
    if ((T) == Nil) return Nil;
    
    int cmp = strcmp(key, Key(T));
    
    if (cmp < 0) {
        Left(T) = DeleteBST(Left(T), key);
    } else if (cmp > 0) {
        Right(T) = DeleteBST(Right(T), key);
    } else {
        /* Node dengan key ditemukan */
        
        /* Case 1: Node adalah daun */
        if (Left(T) == Nil && Right(T) == Nil) {
            DealokasiBST(&T);
            return Nil;
        }
        
        /* Case 2: Node hanya memiliki child kanan */
        if (Left(T) == Nil) {
            AVLTree tmp = Right(T);
            DealokasiBST(&T);
            return tmp;
        }
        
        /* Case 3: Node hanya memiliki child kiri */
        if (Right(T) == Nil) {
            AVLTree tmp = Left(T);
            DealokasiBST(&T);
            return tmp;
        }
        
        /* Case 4: Node memiliki dua child */
        /* Cari successor (minimum di subtree kanan) */
        addressAVL successor = FindMinBST(Right(T));
        
        /* Copy data dari successor ke node T */
        free(Key(T));
        free(WordSunda(T));
        free(Terjemahan(T));
        
        Key(T)        = (char*) malloc(strlen(Key(successor)) + 1);
        WordSunda(T)  = (char*) malloc(strlen(WordSunda(successor)) + 1);
        Terjemahan(T) = (char*) malloc(strlen(Terjemahan(successor)) + 1);
        
        strcpy(Key(T),        Key(successor));
        strcpy(WordSunda(T),  WordSunda(successor));
        strcpy(Terjemahan(T), Terjemahan(successor));
        
        /* Copy sinonim dari successor */
        int i;
        for (i = 0; i < MaxSinonim; i++) {
            if (Sinonim(T)[i] != Nil) {
                free(Sinonim(T)[i]);
            }
            if (Sinonim(successor)[i] != Nil) {
                Sinonim(T)[i] = (char*) malloc(strlen(Sinonim(successor)[i]) + 1);
                strcpy(Sinonim(T)[i], Sinonim(successor)[i]);
            } else {
                Sinonim(T)[i] = Nil;
            }
        }
        
        /* Hapus successor dari subtree kanan */
        Right(T) = DeleteMinBST(Right(T));
    }
    
    /* Update height dan balancing */
    UpdateHeight(T);
    
    int bf = GetBalanceFactor(T);
    
    if (bf > 1 && GetBalanceFactor(Left(T)) >= 0) {
        return RotateRight(T);
    }
    if (bf > 1 && GetBalanceFactor(Left(T)) < 0) {
        return RotateLeftRight(T);
    }
    if (bf < -1 && GetBalanceFactor(Right(T)) <= 0) {
        return RotateLeft(T);
    }
    if (bf < -1 && GetBalanceFactor(Right(T)) > 0) {
        return RotateRightLeft(T);
    }
    
    return T;
}

/* ============================================================
   TRAVERSAL
   ============================================================ */

static void ProsesBST(addressAVL P) {
    if ((P) != Nil) {
        printf("%s | %s\n", Key(P), Terjemahan(P));
    }
}

void InorderBST(AVLTree T) {
    if ((T) != Nil) {
        InorderBST(Left(T));
        ProsesBST(T);
        InorderBST(Right(T));
    }
}

void PreorderBST(AVLTree T) {
    if ((T) != Nil) {
        ProsesBST(T);
        PreorderBST(Left(T));
        PreorderBST(Right(T));
    }
}

void PostorderBST(AVLTree T) {
    if ((T) != Nil) {
        PostorderBST(Left(T));
        PostorderBST(Right(T));
        ProsesBST(T);
    }
}

void PrintTreeBST(AVLTree T, char tab[]) {
    if ((T) != Nil) {
        char newTab[64];
        int i;
        for (i = 0; tab[i] != '\0'; i++) newTab[i] = tab[i];
        newTab[i]   = ' ';
        newTab[i+1] = ' ';
        newTab[i+2] = '\0';
        
        PrintTreeBST(Right(T), newTab);
        printf("%s%s (h=%d)\n", tab, Key(T), Height(T));
        PrintTreeBST(Left(T), newTab);
    }
}

/* ============================================================
   FUNGSI STATISTIK
   ============================================================ */

int NbElmtBST(AVLTree T) {
    if ((T) == Nil) return 0;
    return 1 + NbElmtBST(Left(T)) + NbElmtBST(Right(T));
}

int NbDaunBST(AVLTree T) {
    if ((T) == Nil) return 0;
    if (Left(T) == Nil && Right(T) == Nil) return 1;
    return NbDaunBST(Left(T)) + NbDaunBST(Right(T));
}

int LevelBST(AVLTree T, char *key) {
    if ((T) == Nil) return 0;
    
    int cmp = strcmp(key, Key(T));
    
    if (cmp == 0) return 1;
    
    int lvl = LevelBST(Left(T), key);
    if (lvl != 0) return lvl + 1;
    
    lvl = LevelBST(Right(T), key);
    if (lvl != 0) return lvl + 1;
    
    return 0;
}

/* ============================================================
   VALIDASI BST
   ============================================================ */

static boolean IsValidBSTHelper(AVLTree T, char *minKey, char *maxKey) {
    if ((T) == Nil) return true;
    
    /* Cek apakah Key(T) di dalam range (minKey, maxKey) */
    if ((minKey) != Nil && strcmp(Key(T), minKey) <= 0) return false;
    if ((maxKey) != Nil && strcmp(Key(T), maxKey) >= 0) return false;
    
    /* Rekursif cek left dan right subtree */
    return IsValidBSTHelper(Left(T), minKey, Key(T)) &&
           IsValidBSTHelper(Right(T), Key(T), maxKey);
}

boolean IsValidBST(AVLTree T) {
    return IsValidBSTHelper(T, Nil, Nil);
}

boolean IsValidAVL(AVLTree T) {
    if ((T) == Nil) return true;
    
    int bf = GetBalanceFactor(T);
    if (bf < -1 || bf > 1) return false;
    
    return IsValidAVL(Left(T)) && IsValidAVL(Right(T));
}

/* ============================================================
   CLEANUP
   ============================================================ */

void DestroyBST(AVLTree *T) {
    if ((*T) != Nil) {
        DestroyBST(&Left(*T));
        DestroyBST(&Right(*T));
        DealokasiBST(T);
    }
}
