//
// Created by s257911 on 14/12/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventario.h"


// FUNZIONI PER L'INVENTARIO DEGLI EQUIPAGGIAMENTI
int CheckEmptyInventory(inv_wrapper_p iw_p){
    // controllo che il vettore non sia vuoto
    if(iw_p->dim==0){
        fprintf(stdout,"L'inventario e' vuoto.\n");
        return 0;
    }
    return 1;
}
void PrintInventory(Item *vect,int dim){
    for (int i = 0; i < dim; i++)
        fprintf(stdout,"%d- %s %s %d %d %d %d %d %d\n",i+1,vect[i].name,vect[i].type,
                vect[i].stat.hp,vect[i].stat.mp,vect[i].stat.atk,
                vect[i].stat.def,vect[i].stat.mag,vect[i].stat.spr);
}
void PrintInventory_wrapper(inv_wrapper_p iw_p){
    PrintInventory(iw_p->vect, iw_p->dim);
}
void SearchItemByName(inv_wrapper_p iw_p, char*n){
    int i;
    for(i=0; i<iw_p->dim && strcmp(n,iw_p->vect[i].name)!=0; i++);
    if(i==iw_p->dim){
        fprintf(stdout,"Oggetto non trovato.\n");
        return;
    }
    PrintInventory(&(iw_p->vect[i]),1);
}
void FreeVectItems(inv_wrapper_p iw_p){
    for (int i = 0; i < (iw_p)->dim; i++) {
        free((iw_p)->vect[i].name);
        free((iw_p)->vect[i].type);
    }
    free((iw_p)->vect);
}
void InventorySetVoid(inv_wrapper_p *iw_p){
    (*iw_p)->maxequip=0;
    (*iw_p)->vect=NULL;
    (*iw_p)->dim=0;
}
Item *LoadInventory(FILE *fp,inv_wrapper_p *iw_p){
    char n[size],t[size];
    Item *vect=(Item *)malloc((*iw_p)->dim * sizeof(Item));
    for(int i=0; i<(*iw_p)->dim; i++){
        fscanf(fp,"%s %s %d %d %d %d %d %d",n,t,&vect[i].stat.hp,&vect[i].stat.mp,&vect[i].stat.atk,
               &vect[i].stat.def,&vect[i].stat.mag,&vect[i].stat.spr);
        vect[i].name=strdup(n);
        vect[i].type=strdup(t);
    }
    return vect;
}
void LoadInventory_wrapper(FILE *fp, inv_wrapper_p *iw_p){
    (*iw_p)->maxequip=8;
    fscanf(fp,"%d",&(*iw_p)->dim);
    (*iw_p)->vect=LoadInventory(fp,iw_p);
    fprintf(stdout,"Caricamento avvenuto con successo\n");
}

