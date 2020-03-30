//
// Created by s257911 on 29/12/2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "esArray.h"
#include "esercizio.h"

//enumerazione per le informazioni delle soluzioni
enum informazioni{
    punteggio, difficolta, ultimo_eser, tot
};

//variabile globale per il numero di diagonali e il numero massimo di esercizi per diagonale
int n_diag=3;
int max_eser=5;

//dichiarazzione della struct wrapper
struct esArray{
    Esercizio *vet;
    int dim;
};

esArray_p esArray_init(){
    esArray_p temp;
    temp=(esArray_p)malloc(sizeof *temp);
    temp->vet=NULL;
    temp->dim=0;
    return temp;
}

void esArray_free(esArray_p t){
    int i;
    for(i=0;i<t->dim;i++)
        es_Delete(&t->vet[i]);
    free(t->vet);
    free(t);
}

void esArray_read(FILE *fp, esArray_p t){
    int i;
    fscanf(fp,"%d",&t->dim);
    t->vet=(Esercizio *)malloc(t->dim * sizeof(Esercizio));
    for(i=0;i<t->dim;i++)
        t->vet[i]=es_Read(fp);
}

void esArray_print(FILE *fp, esArray_p t){
    int i;
    for(i=0;i<t->dim;i++)
        es_Print(fp,t->vet[i]);
}

//creazione e inizializzazione della soluzione
int **SolInit(){
    int **temp;
    //allocazione della matrice
    temp=(int **)malloc(n_diag * sizeof(int *));
    for (int i=0; i<n_diag; i++)
        temp[i]=(int *)malloc(max_eser * sizeof(int));

    //inizializzazione con indici non validi
    for(int i=0; i<n_diag; i++)
        for(int j=0; j<max_eser; j++)
            temp[i][j]=-1;

    return temp;
}

//distruzione della matrice soluzione
void FreeSol(int **sol){
    for(int i=0; i<n_diag; i++)
        free(sol[i]);
    free(sol);
}

//creazione e inizializzazione delle matrici di informazioni
float **InfoInit(){
    float **temp;

    //allocazione della matrice
    temp=(float **)malloc(n_diag * sizeof(float *));
    for(int i=0; i<n_diag; i++){
        temp[i]=(float *)malloc(tot * sizeof(float));

        //inizializzazione
        temp[i][punteggio]=temp[i][difficolta]=0;
        temp[i][ultimo_eser]=-1;
    }

    return temp;

}

//distruzione delle matrici di informazioni
void FreeInfo(float **matr){
    for(int i=0; i<n_diag; i++)
        free(matr[i]);
    free(matr);
}

//ritorna un esercizio dall'inventario dato un indice
Esercizio EstraiEsercizio(Esercizio *vet, int index){
    return vet[index];
}

//stabilisce la minima difficoltà degli esercizi
int DifficoltaMinima(esArray_p array){
    int min = INT_MAX;
    for(int i=0; i<array->dim; i++)
        if(EstraiDifficolta(EstraiEsercizio(array->vet,i)) < min)
            min = EstraiDifficolta(EstraiEsercizio(array->vet,i));

    return min;
}

//controllo di accettabilità della scelta
int ControllaScelta(int pos, int esercizio, Esercizio *vet, int **sol, float **info){
    int i=pos/max_eser, j=pos%max_eser;

    if(j==0 && ControlloPrimo(EstraiEsercizio(vet,esercizio)) &&
               ControllaDifficolta( (int) info[i][difficolta],EstraiEsercizio(vet,esercizio), DD))
        return 1;
    if(j!=0 && ControlloCompatibili(EstraiEsercizio(vet,sol[i][j-1]) , EstraiEsercizio(vet,esercizio)) &&
               ControllaDifficolta( (int) info[i][difficolta],EstraiEsercizio(vet,esercizio), DD))
        return 1;
    return 0;
}

//funzione per stabilire se occorre cambiare diagonale
int CambiaDiagonale(int pos,int index, float **info, Esercizio *vet, int min){
    /* Occorre cambiare diagonale se:
     *      1- si sceglie un esercizio terminale
     *      2- si è raggiunto il numero massimo di esercizi
     *      3- non si possono aggiungere esercizi
     */
    int diag=pos/max_eser, es=pos%max_eser;
    if(es!=0 && es%(max_eser-1)==0) //raggiunto il numero limite di esercizi
        return 1;
    if(ControlloTerminale(EstraiEsercizio(vet,index)))
        return 1;
    if( (int) info[diag][difficolta] + EstraiDifficolta(EstraiEsercizio(vet,index)) + min > DD)
        return 1;
    return 0;


}

//funzione per stabilire se una diagonale è valida
int DiagonaleValida(int pos, int **sol, Esercizio *vet){
    /* Una diagonale è valida se:
     *      1- la sua difficoltà non super quella limite (controllo eseuito in fase di inserimento)
     *      2- contiene al suo interno almeno un elemento acrobatico
     */

    int diag=pos/max_eser, es=pos%max_eser;
    for(int i=0; i<=es; i++)
        if(Acrobatico(EstraiEsercizio(vet,sol[diag][i])))
            return 1;

    return 0;
}

//controllo che il programma creato sia valido
int ControllaProgramma(int **sol, float **info, Esercizio *vet){
    /* Un programma generato è valido se:
     *      1- la difficoltà complessiva è compatibile con il limite
     *      2- sono presenti un elemento acrobatico avanti e uno indietro, anche in diagonali diverse
     *      3- almeno una diagonale presenta due elementi acrobatici in sequenza
     */

    //calcolo e controllo della difficoltà complessiva
    int diff=0;
    for(int i=0; i<n_diag; i++)
        diff += (int) info[i][difficolta];
    if(diff>DP)
        return 0;

    //controllo che siano presenti un elemento acrobatico avanti e uno indietro
    int avanti=0, indietro=0;
    for(int i=0; i<n_diag; i++){
        for(int j=0; j<max_eser; j++){
            if(sol[i][j]!=-1 && AcrobaticoAvanti(EstraiEsercizio(vet,sol[i][j])))
                avanti++;
            else if(sol[i][j]!=-1 && AcrobaticoIndietro(EstraiEsercizio(vet,sol[i][j])))
                indietro++;
            if(avanti>=1 && indietro>=1)
                break;
        }
    }
    if(avanti<1 || indietro<1)
        return 0;

    //controllo che almeno una diagonale abbia due elementi acrobatici consecutivi
    avanti=0;   //riclo il contatore per utilizzarlo come flag
    for(int i=0; i<n_diag; i++){
        for(int j=0; j<max_eser-1; j++)
            if(sol[i][j]!=-1 && sol[i][j+1] &&
               AcrobaticiConsecutivi(EstraiEsercizio(vet,sol[i][j]),EstraiEsercizio(vet,sol[i][j+1])) ){
                avanti=1;
                break;
            }
    }
    if(avanti!=1)
        return 0;

    //si arriva a questo punto solo se tutte le condizioni sono soddisfatte
    return 1;
}

//controllo che il punteggio genereato sia migliore di quello finora migliore
int PunteggioMigliore(float **bestinfo, int **sol, float **info, Esercizio *vet){
    float attuale=0, assoluto=0;

    //verifico se l'ulitmo elemento dell'ultima diagonale abbia una difficoltà superiore o uguale a 8
    int index = (int) sol[n_diag - 1][ultimo_eser];
    if(EstraiDifficolta(EstraiEsercizio(vet, index)) >= 8)
        info[n_diag-1][punteggio] = info[n_diag-1][punteggio] * 1.5;

    //calcolo il punteggio attuale
    for(int i=0; i<n_diag; i++){
        attuale += info[i][punteggio];
        assoluto += bestinfo[i][punteggio];
    }

    if(attuale>=assoluto)
        return 1;
    else return 0;
}

void DispRip(int **bestsol, float **bestinfo, int **sol, float **info,    //parametri relativi alle soluzioni
             esArray_p array, int pos, int min){
    int diag,es,k;

    //condizione di terminazione
    if(pos>=(n_diag * max_eser)){
        //controllo che la soluzione sia valida e che il punteggio generato sia migliore
        if(ControllaProgramma(sol,info,array->vet) && PunteggioMigliore(bestinfo,sol,info,array->vet)) {
            //copio la soluzione e le informazioni salvate
            for(int i=0; i<n_diag; i++) {
                bestinfo[i][difficolta] = info[i][difficolta];
                bestinfo[i][punteggio] = info[i][punteggio];
                bestinfo[i][ultimo_eser] = info[i][ultimo_eser];
                for (int j = 0; j < max_eser; j++)
                    bestsol[i][j] = sol[i][j];
            }
        }
        return;
    }

    //calcolo degli indici della matrice della soluzione
    diag=pos/max_eser;
    es=pos%max_eser;

    for(k=0; k<array->dim; k++){
        //controllo di accettabilità dell'esercizio
        if(ControllaScelta(pos,k,array->vet,sol,info)){
            sol[diag][es]=k;
            info[diag][punteggio] += (float) EstraiPunteggio(EstraiEsercizio(array->vet,k));
            info[diag][difficolta] += (float) EstraiDifficolta(EstraiEsercizio(array->vet,k));
            info[diag][ultimo_eser] = (float) k;

            //controllo necessità cambiare diagonale e contorollo validità della diagonale
            if(CambiaDiagonale(pos,k,info,array->vet,min) && DiagonaleValida(pos,sol,array->vet))
                DispRip(bestsol,bestinfo,sol,info,array,(diag+1)*max_eser,min);
            //ricorro sulla prossima posizione solo se non è necessario cambiare diagonale
            else if(!CambiaDiagonale(pos,k,info,array->vet,min))
                DispRip(bestsol, bestinfo, sol, info, array, pos+1, min);

            //backtrack
            info[diag][punteggio] -= (float) EstraiPunteggio(EstraiEsercizio(array->vet,k));
            info[diag][difficolta] -= (float) EstraiDifficolta(EstraiEsercizio(array->vet,k));
        }

    }
}

void BestSol(esArray_p array){
    //la soluzione è costruita come una matrice di n_diag * max_eser
    //ed inizialmente contiene degli indici del vettore non validi
    int **bestsol, **sol;

    //per ognuna delle soluzioni è necessario tenere traccia di alcune informazioni
    //il numero di informazioni è rappresentato dall'ultimo parametro (tot) dell'enumerazione
    //Ad ogni soluzione si affianca quindi una matrice di n_diag * tot con le informazioni
    float **bestinfo, **info;

    int mindifficulty = DifficoltaMinima(array);    //difficoltà minima degli esercizi

    //inzializzo le matrici delle soluzioni
    bestsol = SolInit();
    sol = SolInit();

    //inizializzo le matrici delle informazioni
    bestinfo = InfoInit();
    info = InfoInit();

    //trovo la soluzione migliore
    DispRip(bestsol,bestinfo,sol,info,array,0,mindifficulty);

    //stampa della soluzione trovata
    float cont=0;
    printf("\n------------------------------\nDD: %d\tDP: %d\n",DD,DP);
    for(int i=0; i<n_diag; i++)
        cont += bestinfo[i][punteggio];
    printf("Punteggio: %f\n",cont);

    cont=0;
    for(int i=0; i<n_diag; i++)
        cont += bestinfo[i][difficolta];
    printf("Difficolta: %f\n",cont);

    for(int i=0; i<n_diag; i++){
        fprintf(stdout,"\nDiag #%d > %f\n",i,bestinfo[i][punteggio]);
        for(int j=0; j<max_eser; j++){
            if(bestsol[i][j]!=-1 && EstraiPunteggio(EstraiEsercizio(array->vet,bestsol[i][j])) >0 ){
                Esercizio temp = EstraiEsercizio(array->vet,bestsol[i][j]);
                fprintf(stdout,"%s ",temp.nome);
            }
        }
    }




    //distruzione delle matrici soluzione
    FreeSol(bestsol);
    FreeSol(sol);

    //distruzione delle matrici inormazioni
    FreeInfo(bestinfo);
    FreeInfo(info);
}















