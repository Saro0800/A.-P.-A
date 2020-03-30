//
// Created by s257911 on 29/12/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esArray.h"

#define max 101

int main(){
    esArray_p esercizi=esArray_init();
    FILE *fp;
    char file_name[max];

    //apertura file
    fprintf(stdout,"Nome del file di input: ");
    fscanf(stdin,"%s",file_name);
    if(strstr(file_name,".txt")==NULL)
        strcat(file_name,".txt");
    fp=fopen(file_name,"r");
    if(fp==NULL){
        fprintf(stdout,"File non trovato!\n");
        return EXIT_FAILURE;
    }
    //difficoltà per ogni diagonale
    fprintf(stdout,"Difficolta' massima di ogni diagonale: ");
    fscanf(stdin,"%d",&DD);

    //difficoltà massima complessiva del programma
    fprintf(stdout,"Difficolta' massima del programma: ");
    fscanf(stdin,"%d",&DP);

    //acquisizione degli esercizi disponibili
    esArray_read(fp,esercizi);

    //stampa degli esercizi acquisiti
    fprintf(stdout,"\nEsercizi disponibili:\n");
    esArray_print(stdout,esercizi);

    //ricerca della soluzione migliore
    BestSol(esercizi);







    esArray_free(esercizi);

    return EXIT_SUCCESS;
}

