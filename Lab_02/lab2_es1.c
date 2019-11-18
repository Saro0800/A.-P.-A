//
// Created by rosar on 17/11/2019.
//

/*  CREDITS:
 *  Rosario Francesco Cavelli, matricola: s257911
 *
 *      Ciascuna regione è identificata da: un estr. sx, una bas, un'altezza, un'area. Per identificare
 *      una regione si utilizza, quindi, un nuovo tipo di variabile di tipo 'struct' con i dettagli di ciascuna
 *      regione. Per tenere traccia di tutte le regioni si utilizza quindi un vettore di 'struct'; in questo modo
 *      ciascuna regione sarà identifcata da un certo indice.
 *
 *      Per assegnare ciascuna regione ad un opportuna indice si analizza la mappa, precedentemente salvata su una
 *      matrice, casella per casella. Ogni volta che si incontra una casella nera (un 1) si richiamano 4 funzioni:
 *
 *          1- funzione di conteggio della largehzza: restituisce un int che corrisponde al numero di caselle
 *             orizzonatli nere consecutive a partire dall'1 trovato, che costituiscono la larghezza della regione.
 *
 *          2- funzione di conteggio dell'altezza: restituisce un int corrispondente al numero di caselle nere
 *             verticali nere consecutive a partire dall'1 trovato, che costituiscono l'altezza della regione.
 *
 *          3- funzione di eliminazione: questa funzione di tipo void è stata pensata per evitare che una parte di
 *             una regione già esaminata vada ad occupare un'ulteriore posizione nel vettore di 'struct'. A tal
 *             proposito, partendo dall'1 trovato, si sostituiscono tutte le caselle che costituiscono l'area della
 *             regione con degli 0; in questo modo, analizzando una nuova riga non si incotreranno nuovamente degli 1
 *             nella posizione in cui erano stati già analizzati per una data regione.
 *
 *          4- funzione di assegnazione: assegna i valori determinati per estremo, altezza, larghezza e area ad una
 *             certa regione nel vettore. Per realizzare questa funzione si è fatto un uso particolare dei puntatori;
 *             infatti sono stati usati dei puntatori a struct.
 *
 *      Dopo aver attribuito a ciascuna regione un particolare indice del vettore di struct, si eseguono i confronti
 *      per determinare le regioni con le caratteristiche richieste.
 *
 *      Si analizza il vettore di struct tante volte quante sono le caratteristiche richieste; ad ogni analisi si
 *      salva la regione con la caratteristica richiesta in una variabile uguale per tutti i cicli, che viene quindi
 *      sovrascritta ad ogni ciclo. Successivamente si richiama una funzione di stampa per la regione interessata.
 */

#include <stdio.h>
#include <stdlib.h>

#define max 50      //  Massimo valore che possono assumere righe e colonne

typedef struct{
    int ext[2];
    int altezza;
    int base;
    int Area;
}dett;      //  Struttura identificativa delle regioni

int LoadMatrix(int m[max][max],int dim[2]){
    FILE *fp;
    int rig,col;
    fp=fopen("file_lab2\\Mappa.txt","r");
    if(fp==NULL)
        return -1;
    fscanf(fp,"%d %d",&rig,&col);
    /* Le dimenzioni della matrice all'interno del file corrispondono al numero effettivo delle righe e delle colonne.
     * Poichè si vuole salvare l'indice massimo di ognuna delle dimensioni, occorre diminuire di 1 il numero
     * riportato sul file.
     */
    dim[0]=rig--;
    dim[1]=col--;
    for(int i=0;i<dim[0];i++){
        for(int j=0;j<dim[1];j++){
            fscanf(fp,"%d",&m[i][j]);
        }
    }
    fclose(fp);
    return 0;
}       //  F. di caricamento della matrice (f1)
int CountBase(int m[max][max],int rig, int col, int dim[2]){
    /* La larghezza di una regione è intesa come la sequenza orizzontale di 1 trovata a partire
     * dalla cella che ha innescato il richiamo della funzione. Per determinarne la lunghezza si
     * fa uso di un ciclo for che procede fino a quando non si incontra uno 0, oppure fino a quando
     * non si raggiunge la fine della riga.
     */
    int end=0,cont=0;
    for (int i = col; i < dim[1] && end==0; i++){
        if(m[rig][i]==0)
            end=1;
        else
            cont++;
    }
    return cont;
}       //  F. conteggio della larghezza (f2)
int CountHight(int m[max][max],int rig, int col, int dim[2]){
    /* L'altezza di una regione è intesa come la sequenza verticale di 1 trovata a partire
     * dalla cella che ha innescato il richiamo della funzione. Per determinarne l'altezza si
     * fa uso di un ciclo for che procede fino a quando non si incontra uno 0, oppure fino a quando
     * non si raggiunge la fine della colonna.
     */
    int end=0,cont=0;
    for (int i = rig; i < dim[0] && end==0; i++){
        if(m[i][col]==0)
            end=1;
        else
            cont++;
    }
    return cont;
}       //  F. conteggio dell'altezza (f3)
void Delete(int m[max][max],int rig, int col, int bas,int alt){

    for(int i=rig; i<rig+alt;i++){
        for(int j=col; j<col+bas; j++){
            m[i][j]=0;
        }
    }
}       //  F. eliminzione dalla mappa (f4)
void Region(dett *reg,int b, int h, int a,int rig, int col){
    reg->altezza=h;
    reg->base=b;
    reg->Area=a;
    reg->ext[0]=rig;
    reg->ext[1]=col;
}       //  F. assegnazione al vettore (f5)
void PrintMax(dett reg){
    printf("estr. sup SX=<%d,%d> ",reg.ext[0],reg.ext[1]);
    printf("b=%d, ",reg.base);
    printf("h=%d, ",reg.altezza);
    printf("Area=%d\n",reg.Area);
}       //  F. stampa della matrice richiesta (f6)

int main() {
    int signal,matrix[max][max],dim[2];
    int alt, bas, area,k=0;
    dett regione[max],massima;      /* regione[max] è un vettore di strutture sovradimensionato che
                                     * conterrà le informazioni relative ad ogni regione, ciascuna
                                     * con un proprio indice identificativo. */

    //  Si richiama f1
    signal=LoadMatrix(matrix,dim);
    // Gestione degli errori relativi all'apertura del file
    if(signal==-1){
        printf("Mappa non trovata!");
        return (EXIT_FAILURE);
    }

    // Analisi della mappa casella per casella
    for(int i=0;i<dim[0];i++){
        for(int j=0;j<dim[1];j++) {
            if (matrix[i][j]==1){
                bas=CountBase(matrix,i,j,dim);      //  Si richiama f2
                alt=CountHight(matrix,i,j,dim);     //  Si richiama f3
                area=bas*alt;       //  Calcolo dell'area
                Delete(matrix,i,j,bas,alt);     //  Si richiama f4
                Region(&regione[k],bas,alt,area,i,j);       //  Si richiama f5
                k++;        //  Si aumenta il contatore degli indici del vettore di 'struct'
            }
        }
    }
    //Larghezza maggiore
    massima.base=-1;        /* Si assegna un valore negativo alla base poichè esiste
                             * sicuramente almeno una regione con base maggiore*/
    for (int i = 0; i < k; i++){
        if(regione[i].base>massima.base){
            Region(&massima,regione[i].base,regione[i].altezza,regione[i].Area,regione[i].ext[0],regione[i].ext[1]);
        }
    }
    printf("Max Base: ");
    PrintMax(massima);      //  Richiamo f6

    //Area maggiore
    massima.Area=-1;         /* Si assegna un valore negativo all'area poichè esiste
                              * sicuramente almeno una regione con area maggiore*/
    for (int i = 0; i < k; i++){
        if(regione[i].Area>massima.Area){
            Region(&massima,regione[i].base,regione[i].altezza,regione[i].Area,regione[i].ext[0],regione[i].ext[1]);
        }
    }
    printf("Max Area: ");
    PrintMax(massima);      //  Richiamo f6

    //Altezza maggiore
    massima.altezza=-1;      /* Si assegna un valore negativo all'altezza poichè esiste
                              * sicuramente almeno una regione con altezza maggiore*/
    for (int i = 0; i < k; i++){
        if(regione[i].altezza>massima.altezza){
            Region(&massima,regione[i].base,regione[i].altezza,regione[i].Area,regione[i].ext[0],regione[i].ext[1]);
        }
    }
    printf("Max Altezza: ");
    PrintMax(massima);      //  Richiamo f6

    return 0;
}
