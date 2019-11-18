//
// Created by rosar on 17/11/2019.
//

/*  CREDITS
 *  Programmatore: Rosario Francesco Cavelli, matricola s257911
 *
 *  NOTE:
 *  Nel seguente codice si fa uso solamente di una funzione per la rotazione delle righe e un'altra
 *  per la rotazione delle colonne. Questo perchè inserendo un qualsiasi numero di posizioni da ruotare e una
 *  qualsiasi direzione, tramite un semplice ragionamento matematico, si può far ruotare di x posizioni secondo
 *  una direzione stabilita (dx per le righe, giù per le colonne).
 *  Si è seguito il seguente ragionamento:
 *      1- Supponiamo di inserire pos=P e la direzione 'Sinistra'.
 *      2- Se P<colonne -> P/colonne<1, significa che si effettueranno P rotazioni senza mai tornare alla configurazione
 *         iniziale; quindi è possibile effettuare P=col-P rotazioni verso DX per ottenere un risultato analogo a quello
 *         voluto.
 *      3- Se P>colonne -> P/colonne>1, signifinca che si effettueranno P rotazioni ottenendo esattamente 'P/colonne'
 *         volte la combiazione iniziale. Per ricondursi al caso 2 occorre quindi considerare solo le rotazioni effettivamente
 *         utili; per far ciò si esegue P-colonne*(P/colonne), per ottenere le rotazioni utili, e
 *         colonne-(P-colonne*(P/colonne)) per convertire le rotazioni necessarie lungo la SX nelle rotazioni necessarie
 *         lungo DX (direzione prestabilita)
 *      Attenzione! Le divisioni riportate sono divisioni tra interi, che quindi restituiscono un intero
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define max_file 20
#define max_dim 30

int LoadMatrix(int M[max_dim][max_dim],int dim[2]);     // Prototipo f. di caricamento della matrice
void S_ToLower(char s[10]);     // Prototipo f. di formattazione delle stringhe
void TurnRow(int M[max_dim][max_dim],int index, int pos, int col);      // Prototipo f. rotazione righe
void TurnColumn(int M[max_dim][max_dim],int index, int pos, int rig);   // Prototipo f. rotazione colonne
void PrintMatrix(int M[max_dim][max_dim],int dim[2]);       // Prototipo f. stampa della matrice richiesta

int main() {
    int matrix[max_dim][max_dim],dim[2];
    char select[10],dir[10];
    int index,pos,signal,exit=0;

    // Funzione di caricamento della matrice
    signal=LoadMatrix(matrix,dim);
    if(signal==-1){
        printf("Errore caricamento file!");
        return (EXIT_FAILURE);
    }

    //  Si itera la richiesta e le istruzioni di manipolazione della matrice fino a quando
    //  non si inserisce la parola "fine"
    do{
        // Si richiede l'inserimento del comando da tastiera
        printf("<selettore> <indice> <direzione> <posizioni>\nComando: ");
        scanf("%s %d %s %d",select,&index,dir,&pos);

        index--;    // Le righe e le colonne sono indicate con indici che vanno da 1 a M
        // In C il primo indice è 0, l'ultimo è M-1, quindi è necessario index--

        //Funzioni di formattazione per correggere eventuali errori d'inserimento
        S_ToLower(select);   //Formattazione del selettore
        S_ToLower(dir);      //Formattazione della direzione

        // Menù di selezione delle opzioni
        if(strcmp(select,"riga")==0 && strcmp(dir,"destra")==0) {
            TurnRow(matrix, index, pos, dim[1]);
            PrintMatrix(matrix,dim);
        } else if(strcmp(select,"colonna")==0 && strcmp(dir,"giu")==0) {
            TurnColumn(matrix, index, pos,dim[0]);
            PrintMatrix(matrix,dim);
        } else if(strcmp(select,"riga")==0 && strcmp(dir,"sinistra")==0) {
            if(pos/dim[1]>1)
                pos=dim[1]-(pos-dim[1]*(pos/dim[1]));
            else
                pos=dim[1]-pos;
            TurnRow(matrix,index,pos,dim[1]);
            PrintMatrix(matrix,dim);
        } else if(strcmp(select,"colonna")==0 && strcmp(dir,"su")==0){
            if(pos/dim[0]>1)
                pos=dim[0]-(pos-dim[0]*(pos/dim[0]));
            else
                pos=dim[0]-pos;
            TurnColumn(matrix,index,pos,dim[0]);
            PrintMatrix(matrix,dim);
        } else if(strcmp(select,"fine")==0)
            exit=1;
        else
            printf("Selettore o direzione non validi!\n");
    }while(exit==0);

    return 0;
}

// Funzione di caricamento della matrice
/* Per salvare le dimensioni della matrice si è usato il dualismo vettore-puntatore:
 * passando by value un vettore, in realta si sta passando alla funzione l'indirizzo di memoria
 * del suo primo elemento
 */
int LoadMatrix(int M[max_dim][max_dim],int dim[2]){
    FILE *fp;
    char nome[max_file];
    //  Si richiede il nome del file
    printf("Inserisci il nome del file (max 30): ");
    scanf("%s",nome);
    strcat(nome,".txt");    // Si aggiunge in coda il formato del file
    fp=fopen(nome,"r");
    if(fp==NULL)
        return -1;
    fscanf(fp,"%d %d",&dim[0],&dim[1]);
    for (int i = 0; i < dim[0]; i++) {
        for (int j = 0; j < dim[1]; j++) {
            fscanf(fp,"%d",&M[i][j]);
        }
    }
    fclose(fp);
    return 0;
}

// Funzione di formattazione delle stringhe
/* E' necessario creare questo tipo di funzione in quanto in C non è presente una funzione di libreria
 * di formattazione delle stringhe a un tipo volutoo (maiusc. o minusc.).
 */
void S_ToLower(char s[10]){
    /* Si legge la stringa carattere per carattere e quello che presente all'indice i-esimo viene
     * sostituito dal corrispondente carattere minuscolo */
    for (int i = 0; i < strlen(s); i++)
        s[i]=tolower(s[i]);
}

// Funzione di manipolazione delle righe
/*(Dir. predefinita di riferimento: verso destra)
 */
void TurnRow(int M[max_dim][max_dim],int index, int pos, int col){
    int i=0,temp;       // i contatore del num. di posizioni eseguite - temp variabile di appoggio per i dati sostituiti
    // Si esegue il ciclo esattamente 'pos' volte, dove pos è il numero di scambi effettivamente necessari
    while(i<pos){
        temp=M[index][col-1];       // Viene temporaneamente salvato l'ultimo valore della riga
        for (int j = col-1; j > 0; j--)     // Si spostano a DX tutti i valori della riga
            M[index][j]=M[index][j-1];
        M[index][0]=temp;           // Si mette in testa alla riga quello che prima del ciclo era l'ultimo valore
        i++;            // Incrementa il contatore delle manipolazioni effettuate
    }
}

// Funzione di manipolazione sulle colonne
/* La funzione è identica a quella di manipolazione delle righe, con l'unica differenza che nella sottostante
 * sono stati invertiti gli indici della matrice
 * (Dir. predefinita di riferimento: verso il basso)
 */
void TurnColumn(int M[max_dim][max_dim],int index, int pos,int rig){
    int i=0,temp;
    while(i<pos){
        temp=M[rig-1][index];
        for (int j = rig-1; j > 0; j--)
            M[j][index]=M[j-1][index];
        M[0][index]=temp;
        i++;
    }
}

// Funzione di stampa della matrice al termine della manipolazione richiesta
void PrintMatrix(int M[max_dim][max_dim],int dim[2]){
    for (int i=0; i<dim[0]; i++) {
        for (int j=0; j<dim[1]; j++)
            printf("%d", M[i][j]);
        printf("\n");
    }
}
