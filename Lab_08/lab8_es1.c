//
// Created by s257911 on 11/12/2019.
//


#include <stdio.h>
#include <stdlib.h>


/* Strutture Dati */

typedef struct{
    int start;
    int end;
}att;     // struttura per l'intervallo di tempo

void LoadFile(att **vet,int *dim){
    FILE *fp;
    att *temp;
    fp=fopen("file_lab8/att1.txt","r");
    if(fp==NULL){
        fprintf(stdout,"File non trovato\n");
        temp = NULL;
        return;
    }

    // lettura e acquisizione della dimensione del vettore
    fscanf(fp,"%d",dim);
    // allocazione del vettore
    temp=(att *)malloc(*dim * sizeof(att));
    // acquisizione delle attività
    for (int i = 0; i < *dim; i++)
        fscanf(fp,"%d %d",&temp[i].start,&temp[i].end);
    *vet = temp;
    fclose(fp);
}
int Check(att *vet, int *sol, int pos){
    int i;
    for(i=0;i<pos;i++){
        if(sol[i]!=0 && (vet[i].start<vet[pos].end && vet[pos].start<vet[i].end))
            return 0;
    }
    return 1;
}
void Powerset_R(int pos, att *vet, int *sol, int dim, int *bestsol, int *bestdur, int duract){
    int i;
    // condizione di terminazione
    if(pos>=dim) {
        if(*bestdur<duract) {
            *bestdur = duract;
            for (i = 0; i < dim; i++)
                bestsol[i] = sol[i];
        }
        return;
    }

    sol[pos]=0;
    Powerset_R(pos+1,vet,sol,dim,bestsol,bestdur,duract);
    if(Check(vet,sol,pos)){
        sol[pos]=1;
        Powerset_R(pos+1,vet,sol,dim,bestsol,bestdur,duract + (vet[pos].end - vet[pos].start));
    }

}
void Powerset_w(att *vet, int dim){
    int *bestsol;           // la soluzione è intesa come un vettore di 0 e 1 che indica se l'elemento
    // i-esimo del vettore è preso nella soluzione oppure no
    int *sol;               // soluzione temporanea
    int bestdur=0,i;

    // allocazione e inizializzazione dei vettori delle soluzioni
    bestsol=(int *)calloc(dim, sizeof(int));
    sol=(int *)calloc(dim, sizeof(int));

    Powerset_R(0,vet,sol,dim,bestsol,&bestdur,0);
/*
    // stampa degli indici della soluzione
    for(i=0;i<dim;i++){
        if(bestsol[i]!=0)
            fprintf(stdout,"%d ",i);
    }
*/
    fprintf(stdout,"\nSoluzione con durata migliore: %d\n",bestdur);
    // stampa delle attività della soluzione
    for(i=0;i<dim;i++){
        if(bestsol[i]!=0)
            fprintf(stdout,"(%d %d)\n",vet[i].start,vet[i].end);
    }
    free(sol);
    free(bestsol);
}

int main(){
    att *vet;           // vettore delle attività
    int dim;            // numero di attività totali

    // caricamento del file e del vettore di attività
    LoadFile(&vet,&dim);
    if(vet==NULL)
        return EXIT_FAILURE;

    Powerset_w(vet, dim);

    free(vet);

    return EXIT_SUCCESS;
}
