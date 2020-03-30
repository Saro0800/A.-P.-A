//
// Created by rosar on 19/12/2019.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pg.h"
#include "pgList.h"

typedef struct{
    char cod[7];
}Key;

//alias del puntatore
typedef struct nodoPg_t *link;
//struct del nodo della lista
struct nodoPg_t{
    pg_t infoPg;
    link next;
};

//wrapper dell'ADT
struct pgList_s{
    link head;
    link tail;
    int nPg;
};

//Funzioni per le chiavi
Key KeygetCod(pg_t pg){
    Key k;
    strcpy(k.cod,pg.cod);
    return k;
}

int KeygeqCod(Key k1, Key k2){
    if(strcmp(k1.cod,k2.cod)>=0)
        return 1;
    else return 0;
}

int KeyeqCod(Key k1, Key k2){
    if(strcmp(k1.cod,k2.cod)==0)
        return 1;
    else return 0;
}


//Funzioni ADT
// creatore e distruttore
pgList_p pgList_init(){
    pgList_p temp;
    temp = (pgList_p)malloc(sizeof(*temp));
    temp->head=NULL;
    temp->tail=NULL;
    temp->nPg=0;
    return temp;
}
void pgList_freeNode(link x){
    link temp;
    //condizione di terminazione
    if(x==NULL)
        return;
    //discesa ricorsiva
    pgList_freeNode(x->next);
    temp=x;
    free(x);
    //pulizia delle informazioni del pg
    pg_clean(&temp->infoPg);

/*  Perchè non funziona in questo modo?
    //discesa ricorsiva
    pgList_freeNode(x->next);
    //pulizia delle informazioni del pg
    pg_clean(&x->infoPg);
    free(x);
*/
}
void pgList_free(pgList_p pgList){
    //chiama la funzione ricorsiva di deallocazione dei nodi della lista
    pgList_freeNode(pgList->head);
    free(pgList->head);
    free(pgList->tail);
    free(pgList);
}

link pgList_NewNode(link h, pg_t pg){
    link temp = (link)malloc(sizeof *temp);
    if(temp==NULL){
        printf("Impossibile inserire il personaggio (memoria insufficiente)\n");
        return NULL;
    }
    temp->infoPg=pg;
    temp->next=h;
    return temp;
}

link pglist_SortListIns(link h, pg_t pg){
    link x, p;
    Key k = KeygetCod(pg);

    //inserimento in testa
    if(h==NULL || KeygeqCod(KeygetCod(h->infoPg),k))
        return pgList_NewNode(h,pg);

    //ricerca della posizione
    for(p=h,x=h->next; x!=NULL && KeygeqCod(k,KeygetCod(x->infoPg));p=x,x=x->next);
    p->next=pgList_NewNode(x,pg);
    return h;
}

// inserimento di un nuovo personaggio
void pgList_insert(pgList_p pgList, pg_t pg){
    pgList->head = pglist_SortListIns(pgList->head,pg);
}

link SetTail(link h){
    link x;
    for(x=h; x!=NULL; x=x->next);
    return x;
}
// lettura e scrittura su file
void pgList_read(FILE *fp, pgList_p pgList){
    //dichiaro una struttura temporanea nella quale salvare i dati acquisiti
    pg_t temp;
    while(pg_read(fp,&temp)>0){
        pgList_insert(pgList,temp);
        pgList->nPg++;
    }
    pgList->tail = SetTail(pgList->head);
}
void pgList_print(FILE *fp, pgList_p pgList, tabInv_p invArray){
    link x;
    for (x=pgList->head; x!=NULL; x=x->next)
        pg_print(fp,&x->infoPg,invArray);
}

void ListExtractionByKey(tabInv_p invArray,link *hp, Key k){
    link *xp, t;
    pg_t temp;
    for(xp=hp; (*xp)!=NULL; xp=&((*xp)->next)){
        if(KeyeqCod(k,KeygetCod((*xp)->infoPg))){
            t=*xp;
            *xp=(*xp)->next;
            temp=t->infoPg;
            printf("Personaggio rimosso:\n");
            pg_print(stdout,&temp,invArray);
            pg_clean(&t->infoPg);
            break;
        }
    }
}

// cancellazione con rimozione
void pgList_remove(pgList_p pgList, char* cod, tabInv_p invArray){
    Key k; strcpy(k.cod,cod);
    ListExtractionByKey(invArray, &pgList->head, k);
    pgList->nPg--;
}

// ricerca per codice, ritornando il puntatore
pg_t *pgList_searchByCode(pgList_p pgList, char* cod){
    link x;
    Key k; strcpy(k.cod,cod);
    for(x=pgList->head; x!=NULL && !KeyeqCod(k,KeygetCod(x->infoPg)); x=x->next);
    if(x==NULL){
        printf("PG non trovato\n");
        return NULL;
    }
    return &(x->infoPg);
}

int pgList_Empty(pgList_p pgList){
    if(pgList->nPg==0) {
        printf("Lista PG vuota\n");
        return 1;
    }
    else return 0;
}





