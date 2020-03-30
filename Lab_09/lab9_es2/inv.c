//
// Created by s257911 on 18/12/2019.
//

#include "inv.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define lenght 101

void statPrint(FILE *fp, stat_t *x){
    fprintf(fp,"\t%d %d %d %d %d %d\n",x->hp,x->mp,x->atk,x->def,x->mag,x->spr);
}

void statInput(FILE *fp, stat_t* x){
    fscanf(fp,"%d %d %d %d %d %d",&x->hp,&x->mp,&x->atk,&x->def,&x->mag,&x->spr);
}

void freestringInv(inv_t *x){
    free(x->nome);
    free(x->tipo);
}

// funzioni di input/output di un oggetto dell'inventario
void invInput(FILE *fp, inv_t *x){
    char name[lenght], type[lenght];
    fscanf(fp,"%s %s",name,type);
    x->nome = strdup(name);
    x->tipo = strdup(type);
    statInput(fp,&x->statOgg);
}
void invPrint(FILE *fp, inv_t *x){
    fprintf(fp,"%s %s ",x->nome,x->tipo);
    statPrint(fp,&x->statOgg);
}

/* ritorna il campo stat di un oggetto dell'inventario */
stat_t invgetStat(inv_t *x){
    return x->statOgg;
}