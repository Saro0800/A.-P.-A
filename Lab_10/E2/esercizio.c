//
// Created by s257911 on 29/12/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esercizio.h"

#define max 101

Esercizio es_Read(FILE *fp){
    Esercizio temp;
    char nome[max];
    fscanf(fp,"%s %d %d %d %d %d %f %d\n",nome,&temp.tipologia,&temp.direzione_ingresso,&temp.direzione_uscita,
            &temp.precedenza,&temp.finale,&temp.punteggio,&temp.difficolta);
    temp.nome=strdup(nome);
    return temp;
}

void es_Delete(Esercizio *t){
    free(t->nome);
}

void es_Print(FILE *fp,Esercizio t){
    fprintf(fp,"\n%s\n"
               "\ttipologia:%d\n"
               "\tdirezione ingresso:%d\n"
               "\tdirezione_uscita:%d\n"
               "\tprecedenza:%d\n"
               "\tfinale:%d\n"
               "\tpunteggio:%f\n"
               "\tdifficolta':%d\n",t.nome,t.tipologia,t.direzione_ingresso,t.direzione_uscita,
           t.precedenza,t.finale,t.punteggio,t.difficolta);
}

int EstraiDifficolta(Esercizio es){
    return es.difficolta;
}

float EstraiPunteggio(Esercizio es){
    return es.punteggio;
}

int Acrobatico(Esercizio es){
    if(es.tipologia != 0)
        return 1;
    else return 0;
}

int AcrobaticoAvanti(Esercizio es){
    if(es.tipologia==2)
        return 1;
    return 0;
}

int AcrobaticoIndietro(Esercizio es){
    if(es.tipologia==1)
        return 1;
    return 0;
}

int AcrobaticiConsecutivi(Esercizio es1, Esercizio es2){
    //controllo che siano compatibili
    if(ControlloCompatibili(es1, es2) && Acrobatico(es1) &&Acrobatico(es2))
        return 1;
    return 0;
}

int ControlloPrimo(Esercizio es){
    //il primo esercizio non deve essere preceduto da nulla e deve avere
    //una posizione di ingresso frontale
    if(es.precedenza==0 && es.direzione_ingresso==1)
        return 1;
    return 0;
}

int ControllaDifficolta(int attuale, Esercizio es, int limite){
    if(attuale + EstraiDifficolta(es) <= limite)
        return 1;
    return 0;
}

int ControlloCompatibili(Esercizio es1, Esercizio es2){
    if(es1.direzione_uscita == es2.direzione_ingresso)
        return 1;
    else
        return 0;
}

int ControlloTerminale(Esercizio es){
    return es.finale;
}




