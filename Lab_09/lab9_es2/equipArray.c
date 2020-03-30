//
// Created by s257911 on 18/12/2019.
//

#include <stdlib.h>
#include <stdlib.h>
#include "invArray.h"

#define maxEquip 8
#define lenght 51

// si accede alla struttura dati vera e prorpia (vettore di riferimenti)
// tramite un wrapper contenente le informazioni utili
typedef struct equipArray *equipArray_p;

struct equipArray{
    int inUso;
    int vettEquip[maxEquip];
};

// Funzioni dell'ADT

//creatore e disruttore
equipArray_p equipArray_init(){
    equipArray_p t;

    t = (equipArray_p)malloc(sizeof (*t));
    t->inUso=0;
    for (int i=0; i<maxEquip ; i++)
        t->vettEquip[i]=-1;
    return t;
}
void equipArray_free(equipArray_p equipArray){
    free(equipArray);
}

//quanti equipaggiamenti sono in uso
void equipArray_inUse(equipArray_p equipArray){
    equipArray->inUso=0;
    for (int i=0; i<maxEquip ; i++)
        equipArray->vettEquip[i]!=-1 ? equipArray->inUso++ : (equipArray->inUso+=0);
}

//scrittura su file
void equipArray_print(FILE *fp, equipArray_p equipArray, tabInv_p invArray){
    for(int i=0; i<maxEquip; i++){
        if(equipArray->vettEquip[i]!=-1){
            printf("\t");
            invArray_printByIndex(fp,invArray,equipArray->vettEquip[i]);
        }

    }
}

//torna indice (nel vettore inventario) dell'oggetto in posizione index (0..EQUIP_SLOT-1) di equipArray
int equipArray_getEquipByIndex(equipArray_p equipArray, int index){
    return equipArray->vettEquip[index];
}

//modifica equipaggiamento aggiungendo un oggetto da inventario
void equipArray_add(equipArray_p equipArray, tabInv_p invArray){
    char nome[lenght];
    int i;
    //controllo che ci sia spazio nell'inventario
    if(equipArray->inUso>=maxEquip){
        fprintf(stdout,"Inventario pieno.\n");
        return;
    }
    //richiedo il nome dell'oggetto
    fprintf(stdout,"Nome dell'oggetto: ");
    fscanf(stdin,"%s",nome);

    //cerco il primo slot libero per salvare l'indice
    for (i=0; i<maxEquip && equipArray->vettEquip[i]!=-1; i++);

    //chiamola funzione che ritorna l'indice di un oggetto in base al nome
    if(invArray_searchByName(invArray,nome)<maxEquip) {
        equipArray->vettEquip[i] = invArray_searchByName(invArray, nome);
        equipArray->inUso++;
    } else printf("Oggetto non trovato.\n");

}

//modifica equipaggiamento rimuovendo un oggetto da inventario
void equipArray_remove(equipArray_p equipArray, tabInv_p invArray){
    char nome[lenght];
    int i;
    //controllo che l'inventario non sia vuoto
    if(equipArray->inUso<=0){
        fprintf(stdout,"Inventario vuoto.\n");
        return;
    }
    //richiedo il nome dell'oggetto
    fprintf(stdout,"Nome dell'oggetto: ");
    fscanf(stdin,"%s",nome);

    for (i=0; i<maxEquip && equipArray_getEquipByIndex(equipArray,i) != invArray_searchByName(invArray,nome); i++);
    if(i==maxEquip){
        printf("Oggetto non trovato.\n");
        return;
    }
    equipArray->vettEquip[i] = -1;
    equipArray->inUso--;
}

//verifica che lo slot dell'equipaggiamento non sia vuoto
int equipArray_SlotEmpty(equipArray_p equipArray, int index){
    if(equipArray->vettEquip[index]==-1)
        return 1;
    else return 0;
}