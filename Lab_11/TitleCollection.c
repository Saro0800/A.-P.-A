//
// Created by s257911 on 19/01/2020.
//

#include "TitleCollection.h"
#include <stdio.h>
#include <stdlib.h>

struct titlecollection{ //collezioni di titoli
    int N;      //numero di titoli in lista
    Title head; //lista di titoli
};

TitleList TITLE_LISTinit(){
    TitleList t;
    t=malloc(sizeof *t);
    t->N=0;
    t->head=NULL;
    return t;
}

static void TITLE_LISTfreeR(Title x){
    if(x == NULL)
        return;

    TITLE_LISTfreeR(TITLEnext(x));
    TITLEfree(x);
}

void TITLE_LISTfree(TitleList t){
    TITLE_LISTfreeR(t->head);
    free(t);
}

static Title TITLE_LISTsortins(Title head, Title temp){
    Title x,p;

    //inserimento in testa
    if(head==NULL || TITLEcodgrt(head,temp))
        return TITLEnew(temp, head);

    //ricerca della posizione
    for(x=TITLEnext(head), p=head; x!=NULL && !TITLEcodgrt(x,temp); p=x, x=TITLEnext(x));
    TITLEsetnext(p,TITLEnew(temp,x));
    return head;
}

void TITLE_LISTload(TitleList Titles, FILE *fp){
    //acquisizione del numero di Titoli
    fscanf(fp,"%d",&Titles->N);

    for(int i=0; i<Titles->N; i++)
        //inserimento del titolo nella lista ordinata per codice
        Titles->head = TITLE_LISTsortins(Titles->head,TITLEread(fp));
}

void TITLE_LISTprint(TitleList Titles){
    Title x;
    for(x=Titles->head; x!=NULL; x=TITLEnext(x)){
        TITLEprint(x);
    }
}

int TITLE_LISTvoid(TitleList t){
    if(t->head==NULL)
        return 1;
    return 0;
}

static Title TITLE_LISTfind(TitleList Titles, Title x){
    Title t;
    for(t=Titles->head; t!=NULL; t=TITLEnext(t))
        if(TITLEcodeq(t,x))
            return t;
    return NULL;
}



void TITLE_LISTupdate(TitleList Titles, FILE *fp){
    int n;  //lettura del numero di titoli del file
    Title temp;

    fscanf(fp,"%d",&n);
    for(int i=0; i<n; i++){
        //leggo il titolo
        temp = TITLEread(fp);
        //se il codice non è presente nella lista, lo aggiungo
        if(TITLE_LISTfind(Titles,temp)==NULL){
            Titles->N++;
            //inserimento del titolo nella lista ordinata per codice
            Titles->head = TITLE_LISTsortins(Titles->head,temp);
        }
        //altrimenti aggiorno le quotazioni
        else{
            TITLEupdate(TITLE_LISTfind(Titles,temp),temp);
        }
    }

}

static Title TITLE_LISTsearchpointer(TitleList Titles){
    fprintf(stdout,"Cod: ");
    Title temp = TITLEread(stdin),p;
    if((p = TITLE_LISTfind(Titles,temp))==NULL)
        fprintf(stdout,"Titolo non trovato\n");
    return p;
}

void TITLE_LISTsearch(TitleList Titles){
    Title p = TITLE_LISTsearchpointer(Titles);
    if(p!=NULL)
        TITLEprint(p);
}

void TITLE_LISTsearchquot(TitleList Titles){
    Title p = TITLE_LISTsearchpointer(Titles);
    if(p!=NULL)
        TITLEsearchquot(p);
}

void TITLE_LISTsearchintervalquot(TitleList Titles, int n){
    Title p = TITLE_LISTsearchpointer(Titles);
    if(p!=NULL)
        TITLEsearchquotintervall(p,n);
}

void TITLE_LISTbalanceBST(TitleList Titles){
    Title p = TITLE_LISTsearchpointer(Titles);
    if(p!=NULL)
        TITLEbalanceBST(p);
}



