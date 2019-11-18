//
// Created by Rosario on 18/11/2019.
//

#include <stdio.h>
#include <stdlib.h>

typedef enum{
    bianca, nera
}tipo;

int **malloc2d(int *nr, int *nc,int *signal){       // funzione di caricamento della matrice dinamica
    FILE *fp;
    int **m,i,j;
    // File contenente la matrice
    fp=fopen("file_lab5\\mat.txt","r");
    if(fp==NULL) {
        *signal = -1;
        return NULL;
    }

    // lettura dele dimensioni della matrice
    fscanf(fp,"%d %d",nr,nc);

    // allocazione della matice
    // step 1: allocazione del vettore di puntatori
    m=(int **)malloc(*nr * sizeof(int *));
    // step 2: allocazione dei vettori puntati
    for(i=0;i<*nr;i++)
        m[i]=(int *)malloc(*nc * sizeof(int));

    // assegnazione dei valori
    for(i=0;i<*nr;i++){
        for(j=0;j<*nc;j++)
            fscanf(fp,"%d",&m[i][j]);
    }

    // restituzione della matrice
    return m;

}
void separa(int **mat, int **b, int **w, const int nr, const int nc,int *dim_b, int *dim_w){
    int *blacks, *whites;   // vettori rispettivamente delle "caselle nere" e delle "caselle bianche"
    int i,j;                // indici di scansione della matrice
    int b_w;                // indice delle posizioni delle caselle
    tipo prima=bianca;      // colore della prima casella
    int b_index,w_index;    // indici di scansione dei vettori "nero" e "bianco" rispettivamnete

    /* In una scacchiera (matrice) nr x nc il numero di caselle nere è dato dal risultato della divisione intera
     * tra (nr*nc)/2, se si assume che la prima casella è bianca; in maniera duale, il numero di caselle
     * bianche è la differenza tra nr*nc e il numero calcolato in precedenza.
     *
     * All'interno di una riga della scacchiera si ha, ovviamente, un'alternanza di caselle bianche e di caselle nere.
     * Tuttavia, se il numero di colonne è pari, il colore della casella in posizione [i][nc-1] è uguale a quella in
     * posizione [i+1][0]. Per garantire la corretta alternanza delle caselle occorre ricordare il colore della
     * prima casella; in base ad esso, si è deciso di far iniziare l'indice di posizione della casella da 0, quindi con
     * una casella bianca, oppure con 1, quindi con una casella nera. Indipendentemente dall'indice di inizio si è
     * stabilita una corrispondenza tra indice pari-> casella bianca, indice dispari->casella nera.
     */

    *dim_b=(nr*nc)/2;       // dimensione del vettore "bianco"
    *dim_w=nr*nc - *dim_b;  // dimensione del vettore "nero"

    blacks=(int *)malloc(*dim_b * sizeof(int));     // allocazione del vettore "nero"
    whites=(int *)malloc(*dim_w * sizeof(int));     // allocazione del vettore "bianco"

    b_index=w_index=0;      // inizializzazione degli indici
    // scanzione della matrice e asseganzione ai vettori "nero" e "bianco"
    for(i=0;i<nr;i++){
        if(prima==bianca) {     // prima casella bianca
            b_w = 0;            // l'indice inizia da 0 (bianche in posizione pari)
            prima = nera;       // la prossima prima casella è nera
        }
        else{                   // prima casella nera
            b_w=1;              // l'inidce inizia da 1 (nere in posizione dispari)
            prima=bianca;       // la prossima prima casella è bianca
        }
        for(j=0;j<nc;j++){
            if(b_w%2==0){                       // caso in cui la j-esima casella sia "bianca"
                whites[w_index++]=mat[i][j];    // assegnazione al vettore "bianco"
                b_w++;                          // l'inidice delle caselle avanza
            }
            else{                               // caso in cui la j-esima casella sia "nera"
                blacks[b_index++]=mat[i][j];    // assegnazione al vettore "nero"
                b_w++;                          // l'inidice delle caselle avanza
            }
        }
    }
    *w=whites;      // ritorno l'indirizzo del vettore "bianco"
    *b=blacks;      // ritorno l'indirizzo del vettore "nero"
}
void separa_wrapper(int **matr, int nr,int nc){
    int *blacks, *whites;   // vettori rispettivamente delle "caselle nere" e delle "caselle bianche"
    int dim_b, dim_w;       // dimensioni dei vettori "nero" e "bianco" rispettivamente
    int i;

    // assegnazione iniziale dei vettori "nero" e"bianco"
    blacks=whites=NULL;

    // chiamata della funzioe separa
    separa(matr,&blacks,&whites,nr,nc,&dim_b,&dim_w);

    // stampa dei vettori
    printf("\nVettore Nero:\n");
    for(i=0;i<dim_b;i++)
        printf("%d ",blacks[i]);
    printf("\nVettore Bianco:\n");
    for(i=0;i<dim_w;i++)
        printf("%d ",whites[i]);

    // deallocazione dei vettori
    free(blacks);
    free(whites);

}

int main()
{
    int **matr=NULL;     // matrice di interi   (vettore di vettori tramite puntatori inizializzati a NULL)
    int nr,nc,signal=0,i;

    // Allocazione della matrice e lettura del file
    matr = malloc2d(&nr,&nc,&signal);

    // controllo la corretta lettura del file
    if(signal==-1){
        printf("File non presente\n");
        return EXIT_FAILURE;
    }

    // stampa della matrice
    printf("Matrice:\n");
        for (i = 0; i < nr; i++){
            for (int j = 0; j < nc; j++)
                printf("%d ",matr[i][j]);
            printf("\n");
        }

    // chiamata della funzione di separazione
    separa_wrapper(matr,nr,nc);

    // deallocazione della matrice
    for(i=0;i<nr;i++)
        free(matr[i]);
    free(matr);

    return 0;
}
