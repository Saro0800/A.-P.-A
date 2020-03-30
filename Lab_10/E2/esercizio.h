//
// Created by s257911 on 29/12/2019.
//

#ifndef _ESERCIZIO_H
#define _ESERCIZIO_H

#include <stdio.h>
#include <stdlib.h>

//quasi Adt per ogni esercizio
typedef struct{
    char *nome;
    int tipologia;
    int direzione_ingresso;
    int direzione_uscita;
    int precedenza;
    int finale;
    float punteggio;
    int difficolta;
}Esercizio;

//lettura di un esercizio
Esercizio es_Read(FILE *fp);

//stampa di un esercizio
void es_Print(FILE *fp,Esercizio t);

//distruzione di un esercizio
void es_Delete(Esercizio *t);

//ritorna la difficolta di un esercizio
int EstraiDifficolta(Esercizio es);

//ritorna il punteggio di un esericzio
float EstraiPunteggio(Esercizio es);

//controllo che un esercizio sia acrobatico
int Acrobatico(Esercizio es);

//controllo che un esercizio sia acrobatico avanti
int AcrobaticoAvanti(Esercizio es);

//controllo che un esercizio sia acrobatico indietro
int AcrobaticoIndietro(Esercizio es);

//controllo che due esercizi consecutivi siano acrobatici
int AcrobaticiConsecutivi(Esercizio es1, Esercizio es2);

//controlla che un elemento possa essere il primo di una diagonale
int ControlloPrimo(Esercizio es);

//controlla che la difficoltà aggiuntiva non super il limite
int ControllaDifficolta(int attuale, Esercizio es, int limite);

//controllo che due esericizi consecutivi siano compatibili
int ControlloCompatibili(Esercizio es1, Esercizio es2);

//controllo se un esercizio comporta la fine della diagonale
int ControlloTerminale(Esercizio es);

#endif //_ESERCIZIO_H
