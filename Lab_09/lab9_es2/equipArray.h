//
// Created by s257911 on 18/12/2019.
//

#ifndef _EQUIPARRAY_H
#define _EQUIPARRAY_H

#include <stdlib.h>
#include <stdio.h>
#include "invArray.h"

//ADT  di prima classe per il vettore equip. di ogni pg

//headle della struttura dati
typedef struct equipArray *equipArray_p;

// Funzioni dell'ADT
//creatore e disruttore
equipArray_p equipArray_init();
void equipArray_free(equipArray_p equipArray);

//quanti equipaggiamenti sono in uso
void equipArray_inUse(equipArray_p equipArray);

//scrittura su file
void equipArray_print(FILE *fp, equipArray_p equipArray, tabInv_p invArray);

//modifica equipaggiamento aggiungendo un oggetto da inventario
void equipArray_add(equipArray_p equipArray, tabInv_p invArray);

//modifica equipaggiamento rimuovendo un oggetto da inventario
void equipArray_remove(equipArray_p equipArray, tabInv_p invArray);

//torna indice (nel vettore inventario) dell'oggetto in posizione index (0..EQUIP_SLOT-1) di equipArray
int equipArray_getEquipByIndex(equipArray_p equipArray, int index);

inv_t *equipArray_getOggByIndex(equipArray_p equipArray, int index);

//verifica che lo slot dell'equipaggiamento non sia vuoto
int equipArray_SlotEmpty(equipArray_p equipArray, int index);

#endif //_EQUIPARRAY_H

