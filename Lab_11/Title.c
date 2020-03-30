//
// Created by s257911 on 19/01/2020.
//

#include "Title.h"
#include <stdlib.h>
#include <string.h>

#define lenght 21

struct title{
    char *cod;      //codice alfanumerico del titolo
    Date min, max;  //data meno recente e più recente
    QuotBST q_coll; //BST per la ricerca delle quotazioni
    Title next;     //pointer al prossimo titolo (nodo) in lista
};     //struttura di ogni titolo

Title TITLEnext(Title x){
    return x->next;
}

void TITLEfree(Title x){
    free(x->cod);
    QuotBSTfree(x->q_coll);
    free(x);
}

int TITLEcodgrt(Title t1, Title t2){
    if(strcmp(t1->cod,t2->cod)>0)
        return 1;
    return 0;
}

int TITLEcodeq(Title t1, Title t2){
    if(strcmp(t1->cod,t2->cod)==0)
        return 1;
    return 0;
}

Title TITLEnew(Title val, Title next){
    Title x = malloc(sizeof *x);
    x->cod = val->cod;
    x->min = val->min;
    x->max = val->max;
    x->q_coll = val->q_coll;
    x->next = next;
    return x;
}

static Title TITLEinit(){
    Title t = malloc(sizeof *t);
    t->cod = NULL;
    t->q_coll = NULL;
    t->next = NULL;
    return t;
}
Title TITLEread(FILE* fp){
    Title t = TITLEinit();
    char cod[lenght];

    fscanf(fp,"%s",cod);
    t->cod = strdup(cod);
    if(fp!=stdin){
        t->q_coll = QuotBSTread(fp);
        t->min = QuotBSTmindate(t->q_coll);
        t->max = QuotBSTmaxdate(t->q_coll);
        t->next = NULL;
    }
    return t;
}

void TITLEprint(Title x){
    fprintf(stdout,"\n%s ",x->cod);
    fprintf(stdout,"\tData max: ");
    DatePrint(x->max);
    fprintf(stdout,"\tData min: ");
    DatePrint(x->min);
    QuotBSTprint(x->q_coll);
}

void TITLEsetnext(Title dest, Title new){
    dest->next=new;
}

void TITLEupdate(Title dest, Title new){
    QuotBSTupdate(dest->q_coll, new->q_coll);
    dest->min = QuotBSTmindate(dest->q_coll);
    dest->max = QuotBSTmaxdate(dest->q_coll);
}

void TITLEsearchquot(Title x){
    Date d;
    fprintf(stdout,"Data (aaaa/mm/gg): ");
    fscanf(stdin,"%d/%d/%d",&d.aaaa,&d.mm,&d.gg);
    QuotBSTsearch(x->q_coll, d);
}

void TITLEsearchquotintervall(Title x, int n){
    Date d1, d2;
    if(n==1){
        fprintf(stdout,"Data1 (aaaa/mm/gg): ");
        fscanf(stdin,"%d/%d/%d",&d1.aaaa,&d1.mm,&d1.gg);
        fprintf(stdout,"Data2 (aaaa/mm/gg): ");
        fscanf(stdin,"%d/%d/%d",&d2.aaaa,&d2.mm,&d2.gg);
    } else{
        d1 = x->min;
        d2 = x->max;
    }

    QuotBSTsearchintervall(x->q_coll,d1,d2);
}

void TITLEbalanceBST(Title x){
    if(x->q_coll == NULL){
        printf("ALbero delle quotazioni vuoto.\n");
        return;
    }
    BSTbalance(x->q_coll);
}