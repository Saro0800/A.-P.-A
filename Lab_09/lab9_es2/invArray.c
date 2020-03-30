//
// Created by s257911 on 18/12/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "invArray.h"
#include "inv.h"

#define lenght 101

// si accede alla struttura dati vera e prorpia (vettore dinamico di oggetti)
// tramite un wrapper con le informazioni utili

// wrapper
struct tabInv{
    int nInv, maxInv;
    inv_t *vett;
};

// Funzioni dell'ADT
tabInv_p tabInvInit(){          //allocatore del vettore
    tabInv_p temp;  //puntatore temporaneo

    temp=(tabInv_p)malloc(sizeof(*temp));   // allocazione del wrapper
    temp->nInv=temp->maxInv=0;
    return temp;
}
void tabInvFree(tabInv_p Inv){       //deallocatore del vettore
    for (int i=0; i<Inv->nInv ; i++){
        freestringInv(&Inv->vett[i]);
        free(&Inv->vett[i]);
    }
    free(Inv);
}

// lettura e scrittura su file
void invArray_read(FILE *fp, tabInv_p invArray){
    fscanf(fp,"%d",&invArray->nInv);    // acquisizione del numero di elementi
    invArray->vett=(inv_t*)malloc(invArray->nInv * sizeof(inv_t));  //allocazione della memoria per tutti gli elementi
    for(int i=0;i<invArray->nInv;i++)
        invInput(fp,&invArray->vett[i]);    //richiamo la funzione di input di un oggetto nell'inventario
}

//ritorna puntatore a oggetto selezionato da indice (nel vettore)
inv_t *invArray_getByIndex(tabInv_p invArray, int index){
    return &invArray->vett[index];
}

//stampa un unico oggetto selezionato da indice (nel vettore)
void invArray_printByIndex(FILE *fp, tabInv_p invArray, int index){
    invPrint(fp,invArray_getByIndex(invArray,index));
}

void invArray_print(FILE *fp, tabInv_p invArray){
    for (int i=0; i<invArray->nInv ; i++){
        invArray_printByIndex(fp,invArray,i);
    }
}

//ritorna indice (nel vettore) a oggetto selezionato da nome
int invArray_searchByName(tabInv_p invArray, char *name){
    int i;
    for (i=0; i<invArray->nInv && strcmp(invArray->vett[i].nome,name)!=0; i++);
    return i;
}

int invArray_Empty(tabInv_p invArray){
    if(invArray->nInv==0){
        printf("Ineventario vuoto\n");
        return 1;
    }
    else return 0;
}



