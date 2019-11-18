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
 *
 *      FUNZIONE DI APPARTENENZA
 *      Per realizzare la funzione di appartenenza si esamina il vettore di struct contenente le informazione di tutti
 *      i rettangoli. Se le coordinate sono contenute tra le coordinate che identificano gli estremi della base, e le
 *      coordinate che identificano gli estremi dell'altezza, allora si è trovato il rettangolo a cui appartiene la
 *      casella inserita e se ne stampano le caratteristiche.
 *      Oss: ogni casella è unica all'interno della matrice, di conseguenza essa apparterrà al più ad un solo
 *      rettangolo: per questo motivo si ricorre ad un'uscita non strutturata dal ciclo di scansione.
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

int LoadMatrix(int m[max][max],int *rig,int *col){
    FILE *fp;
    int nr,nc;
    fp=fopen("file_lab3\\Mappa.txt","r");
    if(fp==NULL)
        return -1;
    fscanf(fp,"%d %d",rig,col);
    /* *rig=nr;
    *col=nc;*/
    for(int i=0;i<*rig;i++){
        for(int j=0;j<*col;j++){
            fscanf(fp,"%d",&m[i][j]);
        }
    }
    fclose(fp);
    return 0;
}       //  F. di caricamento della matrice (f1)
int CountBase(int m[max][max],int n_r, int n_c,int *col){
    int end=0,cont=0;
    for (int i = n_c; i < *col && end==0; i++){
        if(m[n_r][i]==0)
            end=1;
        else
            cont++;
    }
    return cont;
}       //  F. conteggio della larghezza (f2)
int CountHight(int m[max][max],int n_r, int n_c,int *rig){
    int end=0,cont=0;
    for (int i = n_r; i < *rig && end==0; i++){
        if(m[i][n_c]==0)
            end=1;
        else
            cont++;
    }
    return cont;
}       //  F. conteggio dell'altezza (f3)
void Delete(int m[max][max],int n_r, int n_c, int bas,int alt){
    for(int i=n_r; i<n_r+alt;i++){
        for(int j=n_c; j<n_c+bas; j++){
            m[i][j]=7;
        }
    }
}       //  F. eliminzione dalla mappa (f4)
void Region(dett *reg,int b, int h, int a,int n_r, int n_c){
    reg->altezza=h;
    reg->base=b;
    reg->Area=a;
    reg->ext[0]=n_r;
    reg->ext[1]=n_c;
}       //  F. assegnazione al vettore (f5)
void PrintMax(dett reg){
    printf("estr. sup SX=<%d,%d> ",reg.ext[0],reg.ext[1]);
    printf("b=%d, ",reg.base);
    printf("h=%d, ",reg.altezza);
    printf("Area=%d\n",reg.Area);
}       //  F. stampa della matrice richiesta (f6)
int riconosciRegione(dett reg[max],int cr,int cc,int *b,int *h,int ext[2],int *k){
    for (int i = 0; i < *k; i++) {
        if(cr>=reg[i].ext[0] && cr<(reg[i].ext[0]+reg[i].altezza)){
            if(cc>=reg[i].ext[1] && cc<(reg[i].ext[1]+reg[i].base)){
                *b=reg[i].base;
                *h=reg[i].altezza;
                ext[0]=reg[i].ext[0];
                ext[1]=reg[i].ext[1];
                return 1;
            }
        }
    }
    return 0;
}   //  F. ricerca appartenenza (f7)

int main() {
    int signal,matrix[max][max],nr,nc,cr,cc;
    int alt, bas, area,k=0;
    int b,h,ext[2];
    dett regione[max],massima;

    //  Richiamo f1
    signal=LoadMatrix(matrix,&nr,&nc);
    if(signal==-1){
        printf("Mappa non trovata!");
        return (EXIT_FAILURE);
    }

    // Analisi della mappa casella per casella
    for(int i=0;i<nr;i++){
        for(int j=0;j<nc;j++) {
            if (matrix[i][j]==1){
                bas=CountBase(matrix,i,j,&nc);      //  Richiamo f2
                alt=CountHight(matrix,i,j,&nr);     //  Richiamo f3
                area=bas*alt;       //  Calcolo dell'area
                Delete(matrix,i,j,bas,alt);     //  Richiamo f4
                Region(&regione[k],bas,alt,area,i,j);       //  Richiamo f5
                k++;        //  Aumento il contatore degli indici del vettore di 'struct'
            }
        }
    }

    //Chiedo l'inserimento delle cooridinate della casella della matrice
    printf("\nInserisci <r,c>: ");
    scanf("%d,%d",&cr,&cc);
    if((cr<0 || cr>nr) || (cc<0 || cc>nc))  //  Controllo che le coordinate rispettino le dim. della matrice
        printf("<%d,%d> non appartiene alla matrice.\n",cr,cc);
    else if(riconosciRegione(regione,cr,cc,&b,&h,ext,&k))       //  Richiamo f7
        printf("<%d,%d> appartiene al rettangolo con estremo <%d,%d>, base %d e altezza %d\n\n",cr,cc,ext[0],ext[1],b,h);
    else
        printf("<%d,%d> non appartiene ad alcun rettangolo\n\n");

    //Larghezza maggiore
    massima.base=-1;        //  Inizializzo il valore massimo della base
    for (int i = 0; i < k; i++){
        if(regione[i].base>massima.base){
            Region(&massima,regione[i].base,regione[i].altezza,regione[i].Area,regione[i].ext[0],regione[i].ext[1]);
        }
    }
    printf("Max Base: ");
    PrintMax(massima);      //  Richiamo f6

    //Area maggiore
    massima.Area=-1;        //  Inizializzo il valore massimo dell'area
    for (int i = 0; i < k; i++){
        if(regione[i].Area>massima.Area){
            Region(&massima,regione[i].base,regione[i].altezza,regione[i].Area,regione[i].ext[0],regione[i].ext[1]);
        }
    }
    printf("Max Area: ");
    PrintMax(massima);      //  Richiamo f6

    //Altezza maggiore
    massima.altezza=-1;     //  Inizializzo il valore massimo dell'altezza
    for (int i = 0; i < k; i++){
        if(regione[i].altezza>massima.altezza){
            Region(&massima,regione[i].base,regione[i].altezza,regione[i].Area,regione[i].ext[0],regione[i].ext[1]);
        }
    }
    printf("Max Altezza: ");
    PrintMax(massima);      //  Richiamo f6

    return 0;
}
