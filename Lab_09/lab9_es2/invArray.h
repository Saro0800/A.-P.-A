//
// Created by s257911 on 18/12/2019.
//

#ifndef _INVARRAY_H
#define _INVARRAY_H

#include <stdio.h>
#include <stdlib.h>

#include "inv.h"

// ADT di prima classe per ealizzare il vettore dell'inventario degli oggetti

// headle della struttura dati
typedef struct tabInv *tabInv_p;

// Funzioni dell'ADT
tabInv_p tabInvInit();          //allocatore del vettore
void tabInvFree(tabInv_p );     //deallocatore del vettore

// lettura e scrittura su file
void invArray_read(FILE *fp, tabInv_p invArray);
void invArray_print(FILE *fp, tabInv_p invArray);

//stampa un unico oggetto selezionato da indice (nel vettore)
void invArray_printByIndex(FILE *fp, tabInv_p invArray, int index);

//ritorna puntatore a oggetto selezionato da indice (nel vettore)
inv_t *invArray_getByIndex(tabInv_p invArray, int index);

//ritorna indice (nel vettore) a oggetto selezionato da nome
int invArray_searchByName(tabInv_p invArray, char *name);

//verifica vettore vuoto
int invArray_Empty(tabInv_p invArray);

#endif //_INVARRAY_H
