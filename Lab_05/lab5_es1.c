//
// Created by Rosario on 17/11/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define size 256

typedef struct{     //representative struct of the list presented by each friend
    int n_elements;     // number of elements/song
    char **elements;    // double pointer to make an array of strings
}Lista;

Lista *LoadFile(int *n,int *signal){
    int j;      // numero di amici, di liste, dimensione della soluzione
    FILE *fp;
    Lista *l_pt;

    fp=fopen("file_lab5\\brani.txt","r");
    if(fp==NULL){
        printf("File non trovato!\n");
        *signal=-1;
        return NULL;
    }
    // Acquisizione del numero di insiemi;
    fscanf(fp,"%d",n);
    // allocazione dinamica del vettore di struct
    l_pt=(Lista *)malloc(*n * sizeof(Lista));

    /*  Il vettore di stringhe è realizzato con un doppio puntatore a carattere.
     * Utilizzando l'allocazione dinamica della memoria occore prima allocare memoria per il vettore di
     * puntatori, successivament per ogni puntatore si alloca un certo numero di byte tali da rappresentare
     * le stringhe puntate*/


    //lettura del file e assegnazione ad ogni struct
    for(int i=0;i<*n ;i++){
        fscanf(fp,"%d",&l_pt[i].n_elements);  // numero di elementi (distinti) dell'insieme i
        // allocazione degli n_elements puntatori a vettori di char
        l_pt[i].elements=(char **)malloc(l_pt[i].n_elements* sizeof(char *));
        // lettura delle n_elements stringhe
        for(j=0;j<l_pt[i].n_elements;j++){
            l_pt[i].elements[j]=(char *)malloc(size*sizeof(char));
            fscanf(fp,"%s",l_pt[i].elements[j]);
        }
    }
    fclose(fp);
    return l_pt;
}
int princ_molt(int pos, Lista *l_pt, char **sol, int dim, int cont){
    int i;
    if(pos>=dim){
        printf("%d\n",cont+1);
        for(i=0;i<dim;i++) {
            printf("%s", sol[i]);
            printf("\n");
        }
        printf("\n");
        return cont+1;
    }

    for(i=0; i<l_pt[pos].n_elements; i++){
        strcpy(sol[pos],l_pt[pos].elements[i]);
        cont=princ_molt(pos+1,l_pt,sol,dim,cont);
    }
    return cont;
}
int princ_moltWRAPPER(Lista *l_pt,int dim){
    int cont=0,pos;    // posizione della soluzione e della ricorsione
    char **sol=(char**)malloc(dim *sizeof(char*)); // vettore soluzione: vettore di stringhe

    // allocazione dello spazio per ogni stringa costituente la soluzione
    for(int i=0;i<dim;i++)
        sol[i]=(char *)malloc(size * sizeof(char));
    cont=princ_molt(0,l_pt,sol,dim,0);
    for(int i=0;i<dim;i++)
        free(sol[i]);
    free(sol);
    return cont;
}

int main() {
    int signal=0,cont,dim;
    Lista *l_pt=LoadFile(&dim,&signal);
    // chiamo la funzione di caricamento del file
    if(signal==-1)
        return (EXIT_FAILURE);

    // stampa delle scelte possibili tramite principio di moltiplicazione
    cont=princ_moltWRAPPER(l_pt,dim);
    printf("Sono state trovate %d soluzioni",cont);
    free(l_pt);
    return 0;
}