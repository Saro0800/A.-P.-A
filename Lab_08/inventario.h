//
// Created by s257911 on 14/12/2019.
//

#ifndef _INVENTARIO_H
#define _INVENTARIO_H

#define size 51

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int hp, mp, atk, def, mag, spr;
}stat_t;           // struttura delle statische

typedef struct {
    char *name, *type;
    stat_t stat;
}Item;                         // struttura di ogni oggetto dell'inventario

typedef struct inv_wrapper* inv_wrapper_p;        // alias per il puntatore alla struct wrapper dell'inventario
struct inv_wrapper{
    unsigned int dim;       // numero di oggetti (numero di elementi del vettore)
    Item *vect;             // vettore degli oggetti (Item)
    int maxequip;           // numero massimo di oggetti che ogni pg può possedere
}inv_w;                     // stuttura wrapper per l'inventario
typedef struct equip* equip_p;       // alias del puntatore ad ogni struttura dell'inventario
struct equip{
    int n;
    Item InUso;
    Item **vect;
}equip;              // struttura dell'inventario


void SearchItemByName(inv_wrapper_p iw_p, char*n);
int CheckEmptyInventory(inv_wrapper_p iw_p);
void InventorySetVoid(inv_wrapper_p *iw_p);
void PrintInventory_wrapper(inv_wrapper_p iw_p);        // stampa del vettore passato (porzione dell'inventario)
void FreeVectItems(inv_wrapper_p iw_p);        // free del vettore dinamico
void LoadInventory_wrapper(FILE *fp, inv_wrapper_p *iw_p);  //acquisizione da file degli oggetti

#endif //_INVENTARIO_H