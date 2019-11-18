//
// Created by rosar on 17/11/2019.
//

#include <stdio.h>
#include <stdlib.h>

#define maxN 30

void Ruota(int v[maxN], int N, int P, int dir);

int main() {
    int n,vet[maxN],P,Dir;

    //Acquisizione di n
    printf("Dimensione vettore: ");
    scanf("%d",&n);

    //Caricamento vettore
    for(int i=0;i<n;i++){
        printf("Vet[%d]: ",i);
        scanf("%d",&vet[i]);
    }

    //Rotazione vettore
    do{
        //Acquisizione di P,ovvero il numero di celle da ruotare
        printf("\nP (P=0 -> End): ");
        scanf("%d",&P);
        //Se P==0 finisce l'esecuzione
        if(P!=0) {
            //Acquisizione Dir, direzione
            printf("Dir: ");
            scanf("%d", &Dir);
            //Controllo che Dir rappresenti una direzione valida
            if (Dir != -1 && Dir != 1)
                printf("Direzione non valida");
            else {
                //Richiamo la funzione ruota
                Ruota(vet, n, P, Dir);
                printf("\n");
            }
        }
    }while(P>0);
    printf("\nFine Esecuzione.");
    return 0;
}

void Ruota(int v[maxN], int N, int P, int dir){
    int ruotato[N],i,j;
    /*Rotazione a dx
     * Per realizzare tale rotazione si può caricare sul vettore 'ruotato':
     * 1- prima la sequenza che rimane invariata in testa al vettore;
     *    questo caricamento inizierà dal valore con indice P fino a quello con indice N-1
     * 2- successivamente la sequenza da ruotare:
     *    questo caricamento inizierà dall'indice 0 e proseguirà fino a quello con indice P-1
     *    */
    if(dir==-1){
        //Caricamento sequenza invariata
        for(i=0;i<N-P;i++)
            ruotato[i]=v[i+P];

        //Caricamento sequenza spostata
        for(j=0;j<P;j++)
            ruotato[i+j]=v[j];

        //Trasformo vet e lo stampo
        for(i=0;i<N;i++) {
            v[i] = ruotato[i];
            printf("%d", v[i]);
        }
    }
        /*Caso P==1, rotazione a sx
         * corrisponde a portare le ultime P cifre in testa al vettore*/
    else{
        //Caricamento sequenza di testa
        for(i=P; i>0;i--)
            ruotato[P-i]=v[N-i];

        //Caricamento sequenza in coda
        for(i=0;i<N-P;i++)
            ruotato[P+i]=v[i];

        //Trasformo vet e lo stampo
        for(i=0;i<N;i++) {
            v[i] = ruotato[i];
            printf("%d", v[i]);
        }
    }
}








