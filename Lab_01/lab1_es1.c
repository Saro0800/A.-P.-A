//
// Created by rosar on 17/11/2019.
//

#include <stdio.h>
#include <stdlib.h>

#define max 20

// Prototipo della funzione di caricamento della matrice
int LoadMatrix(int mat[max][max],int dim[2]);

int main() {
    int camp[max][max];
    int signal,dim[2],capolista=0;

    /* Si assume che il file presenti nella prima riga due numeri, rispettivamente
     * il numero di squadre (righe) e il numero di giornate (colonne);
     * successivamente è presente la matrice, con i numeri separati da uno o più spazi
     * */

    // Richiamo la funzione di caricamento della matrice
    signal=LoadMatrix(camp,dim);
    // Gestione degli errori
    if(signal==-1) {
        printf("Campionato.txt non trovato!\n");
        return (EXIT_FAILURE);
    } else if(signal==-2){
        printf("Il file contiene un punteggio diverso da 0,1,3!\n");
        return (EXIT_FAILURE);
    }

    /* Si scandisce la matrice elemento per elemento. Il primo ciclo for, quello più esterno,
     * scandisce le righe e quindi le giornate del campionato; il secondo ciclo for esamina
     * tutti gli elementi presenti dentro le righe, e quindi i punteggi acquisiti dalle squadre,
     * e trovo la capolista di ogni giornata, ovvero quella che ha fatto più punti, restituendone
     * l'indice di colonna.
     */
    for (int j=0; j<dim[1]; j++) {
        for (int i=0; i<dim[0]; i++) {
            if (camp[i][j] > camp[capolista][j]) {
                capolista = i;
            }
        }
        printf("La capolista della giornata %d e' la squadra numero %d\n", j, capolista);
        capolista = 0;
    }
    return 0;
}

/*La funzione di caricamento della matrice restituisce un valore booleano; dato
 * che nel lingiaggio C il tipo 'boole' non è implementato, la funzione restituisce
 * un intero: <0 se il caricamento non è andato a buon fine o se il file non è stato
 * aperto correttamente, 1 se è stata eseguita con successo*/
int LoadMatrix(int mat[max][max],int dim[2]){
    FILE *fp;
    int punt;
    fp=fopen("Campionato.txt","r");
    if(fp==NULL)
        return -1;

    fscanf(fp,"%d %d",&dim[0],&dim[1]);
    for (int i=0; i<dim[0]; i++) {
        for (int j=0; j<dim[1]; j++) {
            fscanf(fp, "%d", &punt);
            if(punt!=1 && punt!=0 && punt!=3)
                return -2;
            else
                mat[i][j]=punt;
        }
    }
    fclose(fp);
    return 1;

}
