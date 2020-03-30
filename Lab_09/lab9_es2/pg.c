//
// Created by rosar on 19/12/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "equipArray.h"
#include "inv.h"
#include "invArray.h"
#include "pg.h"

#define lenght 51
#define maxEquip 8

//Funzioni del quasi ADT

//lettura e scrittura su file
int pg_read(FILE *fp, pg_t *pgp){
    char name[lenght], class[lenght];

    if(fscanf(fp,"%s %s %s ",pgp->cod,name,class)==EOF)
        return 0;
    pgp->name=strdup(name);
    pgp->class=strdup(class);

    // chiamo la funzione di allocazione dell'equip.
    pgp->equipPg = equipArray_init();

    //chiamo la funzione di lettura delle statistiche
    statInput(fp,&pgp->statPg);
    return 1;
}
void pg_print(FILE *fp, pg_t *pgp, tabInv_p invArray){
    fprintf(fp,"%s %s %s ",pgp->cod,pgp->name,pgp->class);
    // funzione di stampa delle stat
    statPrint(fp,&pgp->statPg);
    //funzione di stampa dell'equipaggiamento
    equipArray_print(fp,pgp->equipPg,invArray);
}

void pg_clean(pg_t *pgp){
    free(pgp->name);
    free(pgp->class);
    equipArray_free(pgp->equipPg);
    free(pgp);
}

/* modifica personaggio aggiungendo/togliendo un equipaggiamento selezionato da inventario:
di fatto e' sufficiente chiamare l'opportuna funzione dal modulo equipArray */
void pg_updateEquip(pg_t *pgp, tabInv_p invArray,int n){
    if(n==1)
        equipArray_add(pgp->equipPg,invArray);
    else if(n==2)
        equipArray_remove(pgp->equipPg,invArray);
}

//stampa delle statistiche di un personaggio
void pg_printstat(pg_t *pg, tabInv_p invArray, equipArray_p equipArray){
    inv_t *x;
    stat_t ogg, tempPg = pg->statPg;
    int i;

    for(i=0;i<maxEquip; i++){
        if(!equipArray_SlotEmpty(pg->equipPg,i)){
            x=invArray_getByIndex(invArray,equipArray_getEquipByIndex(equipArray,i));
            ogg = invgetStat(x);
            (tempPg.hp + ogg.hp)>0 ? (tempPg.hp+=ogg.hp) : (tempPg.hp=1);
            (tempPg.mag + ogg.mag)>0 ? (tempPg.mag+=ogg.mag) : (tempPg.mag=1);
            (tempPg.atk + ogg.atk)>0 ? (tempPg.atk+=ogg.atk) : (tempPg.atk=1);
            (tempPg.def + ogg.def)>0 ? (tempPg.def+=ogg.def) : (tempPg.def=1);
            (tempPg.mp + ogg.mp)>0 ? (tempPg.mp+=ogg.mp) : (tempPg.mp=1);
            (tempPg.spr + ogg.spr)>0 ? (tempPg.spr+=ogg.spr) : (tempPg.spr=1);
        }
    }
    statPrint(stdout,&tempPg);
}
