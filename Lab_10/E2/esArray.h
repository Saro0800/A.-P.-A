//
// Created by s257911 on 29/12/2019.
//

#ifndef _ESARRAY_H
#define _ESARRAY_H

#include "esercizio.h"

//varibili globali per la difficoltà della diag. e di tutto il programma
int DD, DP;

//dichiarazione dell'headle per ADT di I classe
typedef struct esArray *esArray_p;

//creazione vettore di esercizi
esArray_p esArray_init();

//distruzione vettore
void esArray_free(esArray_p t);

//lettura della collezione di esercizi
void esArray_read(FILE *fp, esArray_p t);

//stampa della collezione di esercizi
void esArray_print(FILE *fp, esArray_p t);

//ricerca della soluzione migliore
void BestSol(esArray_p array);



#endif //_ESARRAY_H
