//
// Created by rosar on 19/12/2019.
//

#ifndef _PGLIST_H
#define _PGLIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pg.h"

// ADT di prima classe collezione di personaggi
typedef struct pgList_s *pgList_p;

//Funzioni ADT
// creatore e distruttore
pgList_p pgList_init();
void pgList_free(pgList_p pgList);

// lettura e scrittura su file
void pgList_read(FILE *fp, pgList_p pgList);
void pgList_print(FILE *fp, pgList_p pgList, tabInv_p invArray);

// inserimento di un nuovo personaggio
void pgList_insert(pgList_p pgList, pg_t pg);

// cancellazione con rimozione
void pgList_remove(pgList_p pgList, char* cod, tabInv_p invArray);

// ricerca per codice, ritornando il puntatore
pg_t *pgList_searchByCode(pgList_p pgList, char* cod);

//verifica lista non vuota
int pgList_Empty(pgList_p pgList);

#endif //_PGLIST_H
