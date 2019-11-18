//
// Created by rosar on 17/11/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20

//  Prototipi delle funzioni
void LoadMatrix(int nr,int nc, int m[MAX][MAX],FILE *fp);                           //F.1 di caricamento della matrice
int SottoMatrici(int dim, int nr, int nc, int m[MAX][MAX], int max, int pos[2]);    //F.2 di stampa delle sottomatrici e determinazione della massima somma
void MaxMatrix(int dim, int pos[2], int m[MAX][MAX], int Massima[MAX][MAX]);        //F.3 di sostituzione della matrice con somma massima


int main() {
    //  DICHIARAZIONE VARIABILI
    int nr,nc,matrix[MAX][MAX],inf,dim,Massima[MAX][MAX],max=-1,sum,pos[2],dim_max;
    FILE *fp;
    char nome[MAX];

    //  Richiesta del nome del file (senza .txt)
    printf("Nome file: ");
    scanf("%s",nome);
    strcat(nome,".txt");

    /*  Apertura del file e controllo di apertura avvenuta con successo
     * Se il puntatore è NULL, il programma viene interrotto*/
    fp=fopen(nome,"r");
    if(fp==NULL){
        printf("Errore File!\n");
        return (EXIT_FAILURE);
    }

    //  Acquisizione delle dimensioni della matrice
    fscanf(fp,"%d %d",&nr,&nc);

    //  Caricamento matrice (F.1)
    LoadMatrix(nr,nc,matrix,fp);
    fclose(fp);

    /*  Stabilisco il limite inferiore, ovvero la dimensione minima tra righe e colonne della matrice caricata
     * è necesserio stabilire questo limite in quanto si stanno richiedendo delle matrici quadrate, ovvero
     * matrici con le stesse dimensioni. Le (la) dimensioni(e) delle matrici quadrate non possono essere superiore
     * alla minima dimensione della matrice generale
     * */
    if(nr<=nc)
        inf=nr;
    else
        inf=nc;

    //Chiedo all'utente di inserire un valore numerico compreso tra 1 e inf.
    //In caso contrario stampo un messaggio di errore e termino l'esecuzione
    do{
        printf("Inserisci un numero (0<n<%d): ",inf);
        scanf("%d",&dim);
        if(dim<1 || dim>inf)
            printf("Valore non valido!\n");
        else{
            /*  Richiamo la funzione di stampa delle sottomatrici (F.2).
             *  A sum è attribiuto il valore restituito dalla funzione, corrispondente alla massima somma trovata
             *  per le sottomatrici con dimensioni dim.
             *  Alla funzione viene inoltre passato il parametro max, il massimo valore trovato fino a quel momento,
             *  che tiene conto di tutte le sottomatrici richieste dall'utente.
             *  Successivamente viene effettuato il confronto tra sum e max per valutare l'aggiornamento del valore
             *  massimo. Se questo valore viene cambiato, significa che occorrerà anche modificare la sottomatrice con
             *  somma massima trovata fino a quel momento, per cui si richiama la funzione di sostituzione (F.3), e
             *  inoltre occorre ricordare la dimensione della matrice in questione, quindi aggiornare il valore di
             *  dim_max
             *  */
            sum=SottoMatrici(dim,nr,nc,matrix,max,pos);
            if(sum>max) {
                max=sum;
                dim_max=dim;
                //Richiamo la funzione di sostituzione della matrice massima
                MaxMatrix(dim,pos,matrix,Massima);
            }
        }
    }while(dim>=1 && dim<=inf);

    //Stampo la ma trice di somma massima, e il valore della somma massima
    for(int i=0;i<dim_max;i++){
        for(int j=0;j<dim_max;j++){
            printf("%d ",Massima[i][j]);
        }
        printf("\n");
    }
    printf("\nSomma massima: %d\n",max);

    return 0;

}

void LoadMatrix(int nr,int nc,int m[MAX][MAX],FILE *fp){
    int i,j;
    //caricamento della matrice
    for(i=0; i<nr; i++){
        for(j=0; j<nc; j++){
            fscanf(fp,"%d",&m[i][j]);
        }
    }


}

int SottoMatrici(int dim, int nr, int nc, int m[MAX][MAX], int max, int pos[2]){
    int i,j,sum,max_temp=max;

    /*Per la stampa delle sottomatrici occore controllare quelle che si originano
     *da ciascun valore della matrice originale.
     *Saranno validi solo quelle per le quali i<=nr-dim e j<=nc-dim
     */

    /*  Per il calcolo della somma:
     * 1- Quando si passa ad esaminare una nuova sottomatrice si azzera il contatore della somma
     * 2- Ogni volta che si stampa un valore della sottomatrice si somma questo valore al valore di
     *    di sum calcolato fino a quel momento per la sottomatrice in esame
     * 3- Al termine dell'esame della sottomatrice, se la sua somma è maggiore del valore massimo riscontrato fino
     *    a quel momento in tutte le matrici precedenti (var. passata come parametro della funzione), alloro aggiorno
     *    il valore max_temp che rappresenta il massimo della somma della sottomatrice in esame e salvo l'indice
     *    del primo valore della sottomatrice in pos[2]
     * 4- Al termine dell'esame di tutte le sottomatrici restituisco il valore trovato per max_temp; questa operazione
     *    serve per restituire al main un valore con il quale è possibile fare un controllo sul valore massimo trovato
     *    fino a quel momento, e stabilire se sia il caso di aggiornare il massimo globale. (Questa operazione potrebbe
     *    essere fatta con i puntatori passati come parametro alla funzione)
     */

    for(i=0; i<=nr-dim; i++){
        for (j = 0; j<=nc-dim; j++) {
            sum=0;
            for (int k = i; k < i + dim; k++) {
                for (int x = j; x < j + dim; x++) {
                    printf("%d ", m[k][x]);
                    sum += m[k][x];
                }
                printf("\n");
            }
            printf("\n");
            if(sum>max_temp){
                max_temp=sum;
                pos[0]=i; pos[1]=j;
            }
        }
    }
    return max_temp;
}

void MaxMatrix(int dim, int pos[2], int m[MAX][MAX], int Massima[MAX][MAX]){
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            Massima[i][j]=m[i+pos[0]][j+pos[1]];
        }
    }
}
