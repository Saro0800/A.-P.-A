//
// Created by s257911 on 18/12/2019.
//

#ifndef _INV_H
#define _INV_H

#include <stdio.h>
#include <stdio.h>

//quasi ADT per le statische di oggetti e dei pg
typedef struct{
    int hp, mp, atk, def, mag, spr;
}stat_t;

// quasi ADT per realizzare il tipo di dato associato a ciascun oggetto
typedef struct{
    char *nome, *tipo;
    stat_t statOgg;
}inv_t;

//Funzioni dei quasi ADT
// funzioni di input/output delle statistiche
void statPrint(FILE *, stat_t*);
void statInput(FILE *, stat_t*);

// funzioni di input/output di un oggetto dell'inventario
void invInput(FILE *, inv_t*);
void invPrint(FILE *, inv_t*);

// deallocatore delle stringhe dinamiche
void freestringInv(inv_t*);

// ritorna il campo stat di un oggetto dell'inventario
stat_t invgetStat(inv_t *);

#endif //_INV_H
