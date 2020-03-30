//
// Created by s257911 on 11/12/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char col;   // colore del segmento di tubo
    int val;    // valore del segmento di tubo
}Tubo;          // struttura per la descrizione dei tubi
typedef struct{
    Tubo o;     // tubo orizzontale
    Tubo v;     // tubo verticale
}Tessera;       // struttura delle tessere
typedef struct{
    int index;  // indice del vettore di tessere
    int rot;    // intero booleano: indica se la tessera è ruotata o no di 90°
}Tile;          // struttura delle caselle della scacchiera

FILE *LoadFile(){
    FILE *fp;
    char nome[101];

    fprintf(stdout,"Nome del file: ");
    fscanf(stdin,"%s",nome);
    if(strstr(nome,".txt")==NULL)
        strcat(nome,".txt");

    fp=fopen(nome,"r");
    if(fp==NULL){
        fprintf(stdout,"File non trovato\n");
        return NULL;
    }

    return fp;
}

Tessera *CaricaTessere(FILE *fp, int *dim){
    int i;
    Tessera *vet;

    fscanf(fp,"%d",dim);

    vet=(Tessera *)malloc(*dim * sizeof(Tessera));
    for(i=0;i<*dim;i++)
        fscanf(fp,"%*c%c %d %c %d",&vet[i].o.col,&vet[i].o.val,&vet[i].v.col,&vet[i].v.val);

    fclose(fp);
    return vet;
}
Tile TileSetVoid(){
    Tile temp;
    temp.index=-1;
    temp.rot=-1;
    return temp;
}
Tile **MallocMatrice(int r, int c){
    Tile **mat;
    int i;

    // allocazione della matrice
    mat=(Tile**)malloc(r * sizeof(Tile *));
    for(i=0;i<r;i++)
        mat[i]=(Tile *)malloc(c * sizeof(Tile));
    return mat;
}
Tile **CaricaScacchiera(FILE *fp, int *mark, int *r, int *c){
    Tile **mat;
    int i,j;
    // acquisizione delle dimensioni
    fscanf(fp,"%d %d",r,c);

    // allocazione della matrice
    mat=MallocMatrice(*r,*c);


    // caricamento delle tessere predefinite
    for(i=0;i<*r;i++){
        for(j=0;j<*c;j++){
            fscanf(fp,"%d/%d ",&mat[i][j].index,&mat[i][j].rot);
            mark[mat[i][j].index]=1;    // segnalazione dell'indisponibilità della tessera
        }
    }

    fclose(fp);
    return mat;
}
void FreeScacchiera(Tile **mat, int r, int c){
    int i;

    for(i=0;i<r;i++)
        free(mat[i]);

    free(mat);
}
int VerificaSoluzione(Tessera *tess, Tile **sol, int r, int c, int *bestcol, int* bestrig){
    int col=0;
    int rig=0;
    int i,j,temp;
    char controll;

    // controllo sulle righe
    for(i=0;i<r;i++){
        if(sol[i][0].rot == 0)      // se la tessera non è ruotata
            controll = tess[sol[i][0].index].o.col;     // seleziono il segmento orizzonatale
        else if(sol[i][0].rot == 1) // se la tessera è ruotata
            controll = tess[sol[i][0].index].v.col;     // seleziono il segmento in verticale
            temp=0;
        for(j=0;j<c;j++){
            // se la tessera corrente non è ruotata e il colore del segmento orizzontale coincide con il colore dei
            // segmenti orizzontali fino ad ora trovati, aggiorno la somma della riga temporanea
            if(sol[i][j].rot==0 && controll==tess[sol[i][j].index].o.col)
                temp += tess[sol[i][j].index].o.val;
            // se la tessera corrente è ruotata e il colore del segmento verticale coincide con il colore dei
            // segmenti orizzontali fino ad ora trovati, aggiorno la somma della riga temporanea
            else if(sol[i][j].rot==1 && controll==tess[sol[i][j].index].v.col)
                temp += tess[sol[i][j].index].v.val;
            // non appena trovo una tessera, ruotata o meno, il cui colore "orizzontale" non coincide
            // con quelli fino ad ora analizzati, interrompo il ciclo e pongo la somma temporanea a 0
            else{
                temp=0;
                break;
            }
        }
        rig+=temp;
    }

    // controllo sulle colonne
    for(j=0;j<c;j++){
        if(sol[0][j].rot==0)    // se la tessera non è ruotata
            controll = tess[sol[0][j].index].v.col; // seleziono il segmento verticale
        else if(sol[0][j].rot==1)       // se la tessera è ruotata
            controll = tess[sol[0][j].index].o.col; // seleziono il segmento orizzontale
        temp = 0;
        for(i=0;i<r;i++){
            // se la tessera corrente non è ruotata e il segmento verticale coincide con quelli verticali
            // analizzati fino ad ora, aggiorno la somma temporanea
            if(sol[i][j].rot==0 && controll==tess[sol[i][j].index].v.col)
                temp += tess[sol[i][j].index].v.val;
            // se la tessera corrente è ruotata e il segmento orizzontale coincide con quelli verticali
            // analizzati fino ad ora, aggiorno la somma temporanea
            else if(sol[i][j].rot==1 && controll==tess[sol[i][j].index].o.col)
                temp += tess[sol[i][j].index].o.val;
            // non appena trovo una tessera, ruotata o meno, il cui colore "verticale" non coincide
            // con quelli fino ad ora analizzati, interrompo il ciclo e pongo la somma temporanea a 0
            else{
                temp=0;
                break;
            }
        }
        col+=temp;
    }

    if(*bestcol<col || *bestrig<rig){
        *bestcol=col;
        *bestrig=rig;
        return 1;
    } else return 0;
}
void disp_sempl(int pos, Tessera *tess, int n_tess, Tile **sol, Tile **bestsol, int *mark,
                int dimsol, int *bestcol, int* bestrig, int nr, int nc){
    int k,i,j;

    // condizione di terminazione
    if(pos>=dimsol){
        if(VerificaSoluzione(tess,sol,nr,nc,bestcol,bestrig)==1){
            for(i=0;i<nr;i++)
                for(j=0;j<nc;j++)
                    bestsol[i][j]=sol[i][j];
        }
        return;
    }

    // calcolo gli indici della posizione corrente
    i=pos/nr; j=pos%nc;

    // controllo che la tessera sia libera: se libera, continuo e opero le scelte,
    // altrimenti ricorro sulla posizione successiva
    if(sol[i][j].index!=-1){
        disp_sempl(pos+1,tess,n_tess,sol,bestsol,mark,dimsol,bestcol,bestrig,nr,nc);
        return;
    }

    // iterazione sul vettore delle caselle
    for(k=0;k<n_tess;k++){
        if(mark[k]==0){
            // seleziono la tessera non ruotata
            mark[k]=1;
            sol[i][j].index=k;
            sol[i][j].rot=0;
            disp_sempl(pos+1,tess,n_tess,sol,bestsol,mark,dimsol,bestcol,bestrig,nr,nc);
            // seleziono la tessera ruotata
            sol[i][j].rot=1;
            disp_sempl(pos+1,tess,n_tess,sol,bestsol,mark,dimsol,bestcol,bestrig,nr,nc);
            // deseleziono la tessera
            mark[k]=0;
            sol[i][j].index=-1;
            sol[i][j].rot=-1;
        }
    }

}
void GeneraSoluzione_w(Tessera *tess, int n_tess, int *mark, Tile **mat, int nr, int nc){

    // per generare le soluzioni e scegliere quella migliore, si utilizza il modello
    // delle disposizioni semplici. Bisogna tuttavia adattare questo modello, che solitamente
    // opera su un vettore soluzione, affinchè possa operare su una matrice delle soluzioni.
    // per far ciò si numerano idealmente le caselle da 0 a nr*nc-1 partendo dalla casella [0][0]
    // in alto a sinistra fino alla casella [nr-1][nc-1] in basso a destra. Conoscendo la posizione
    // della tessere è possibile risalire agli indici della matrice: i=pos/nr e j=pos%nc

    Tile **bestsol;     // soluzione migliore       // inizialmente sono entrambe uguali alla scacchiera predefinita
    Tile **sol;         // soluzione generata
    int bestcol=0, bestrig=0;   // valori migliori (miglior somma delle colonne e miglior somma delle righe)
    int i,j;

    // allocazione della memoria necessaria alle due soluzioni
    bestsol=MallocMatrice(nr,nc);
    sol=MallocMatrice(nr,nc);

    // copia della matrice predefinita
    for(i=0;i<nr;i++)
        for(j=0;j<nc;j++)
            bestsol[i][j]=sol[i][j]=mat[i][j];

    disp_sempl(0,tess,n_tess,sol,bestsol,mark,nr*nc,&bestcol,&bestrig,nr,nc);

    // stampo la soluzione migliore trovata
    fprintf(stdout,"Soluzione migliore trovata:\n");
    for(i=0;i<nr;i++){
        for(j=0;j<nc;j++)
            fprintf(stdout,"%d/%d ",bestsol[i][j].index,bestsol[i][j].rot);
        printf("\n");
    }

}

int main(){
    Tessera *tess;      // vettore dinamico delle tessere
    Tile **scac;        // matrice di tiles: rappresenta la scaccchiera
    int *mark;          // vettore di flag riferito alle tessere: indica se la i-esima tessere è stata già presa o meno
    int n_tess;         // numero di tessere presenti
    int nr, nc;         // dimensioni della scacchiera
    FILE *fp;
    //int i;

    // caricamento del vettore di tessere
    fprintf(stdout,"Caricamento delle tessere.\n\t");
    fp = LoadFile();
    if(fp==NULL)
        return EXIT_FAILURE;
    tess = CaricaTessere(fp,&n_tess);

    // stampa delle tessere
    //for(i=0;i<n_tess;i++)
        //fprintf(stdout,"%c %d %c %d\n",tess[i].o.col,tess[i].o.val,tess[i].v.col,tess[i].v.val);

    // vettore mark per tenere traccia delle tessere disponibili e di quelle prese
    mark=(int *)calloc(n_tess, sizeof(int));

    // caricamento della scacchiera
    fprintf(stdout,"Caricamento della scacchiera\n\t");
    fp = LoadFile();
    if(fp==NULL)
        return EXIT_FAILURE;
    scac = CaricaScacchiera(fp,mark,&nr,&nc);

    GeneraSoluzione_w(tess,n_tess,mark,scac,nr,nc);








    FreeScacchiera(scac,nr,nc);
    free(mark);
    free(tess);


    return EXIT_SUCCESS;
}