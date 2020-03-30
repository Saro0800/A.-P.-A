//
// Created by s257911 on 15/01/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ST.h"

typedef struct {
    char *id_elab;
    char *id_rete;
}Elab;

struct st{ int N; Elab *vet; };

//inizializzazione dela tabella di simboli
ST STinit(int V){
    ST t;
    t=malloc(sizeof *t);
    t->N=0;
    t->vet=malloc(V * sizeof(Elab));
    return t;
}

//distruzione della tabella di simboli
void STfree(ST tab){
    for(int i=0; i<tab->N; i++) {
        free(tab->vet[i].id_elab);
        free(tab->vet[i].id_elab);
    }
    free(tab->vet);
    free(tab);
}

//inserimento nella tabella di simboli
void STinsert(ST tab, char *id_elab, char *id_rete){
    int i=0;
    //controllo che l'elemento non sia già stato inserito
    for(i=0; i<tab->N; i++)
        if(strcmp(tab->vet[i].id_elab,id_elab)==0)
            return;

    //inserisco nella prima posizione disponibile
    tab->vet[i].id_elab=strdup(id_elab);
    tab->vet[i].id_rete=_strdup(id_rete);
    tab->N++;
}

int STcount(ST tab){
    return tab->N;
}

static int Elabcmp(Elab first, Elab second){
    if(strcmp(first.id_elab,second.id_elab)<=0)
        return 1;
    return 0;
}

void STrealloc(ST tab){
    tab->vet = realloc(tab->vet, tab->N * sizeof(Elab));
}

void Merge(Elab *A, Elab *B, int l, int q, int r) {
    int i, j, k;
    i = l;
    j = q+1;
    for(k = l; k <= r; k++)
        if (i > q)
            B[k] = A[j++];
        else if (j > r)
            B[k] = A[i++];
        else if (Elabcmp(A[i], A[j]))
            B[k] = A[i++];
        else
            B[k] = A[j++];
    for ( k = l; k <= r; k++ )
        A[k] = B[k];
    return;
}

void MergeSortR(Elab *A, Elab *B, int l, int r) {
    int q;
    if (r <= l)
        return;
    q = (l + r)/2;
    MergeSortR(A, B, l, q);
    MergeSortR(A, B, q+1, r);
    Merge(A, B, l, q, r);
}

void MergeSort(Elab *A, int N) {
    int l=0, r=N-1;
    Elab *B = (Elab *)malloc(N*sizeof(Elab));
    if (B == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    MergeSortR(A, B, l, r);
}

void STsort(ST tab){
    MergeSort(tab->vet, tab->N);
}

int STsearch(ST tab, char *id_elab){
    //ricerco all'interno della tabella di simboli
    for(int i=0; i<tab->N; i++)
        if(strcmp(tab->vet[i].id_elab,id_elab)==0)
            return i;

    //se la tabella di simboli viene percorsa tutta senza trovre nulla, viene tornato un indice negativo
    return -1;
}

char *STsearchByIndex(ST tab, int i){
    return tab->vet[i].id_elab;
}

