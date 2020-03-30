//
// Created by rosar on 19/12/2019.
//

#ifndef _PG_H
#define _PG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipArray.h"
#include "inv.h"
#include "invArray.h"

#define cod_lenght 7

//Quasi ADT rappresentativo di ogni pg
typedef struct{
    char *name, *class;
    char cod[cod_lenght];
    stat_t statPg;
    equipArray_p equipPg;
}pg_t;

//Funzioni del quasi ADT

//lettura e scrittura su file
int pg_read(FILE *fp, pg_t *pgp);
void pg_print(FILE *fp, pg_t *pgp, tabInv_p invArray);

//pulisce chiamando il distruttire di equipArray
void pg_clean(pg_t *pgp);

/* modifica personaggio aggiungendo/togliendo un equipaggiamento selezionato da inventario:
di fatto e' sufficiente chiamare l'opportuna funzione dal modulo equipArray */
void pg_updateEquip(pg_t *pgp, tabInv_p invArray,int n);

//stampa delle statistiche di un personaggio
void pg_printstat(pg_t *pg, tabInv_p invArray, equipArray_p equipArray);

#endif //_PG_H
