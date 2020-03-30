//
// Created by Rosario on 20/11/2019.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    const int first, second;
}arch;
typedef struct{
    int val;
}vert;

void LoadFile(arch **archi, vert **verts, const int *n_vert, const int *n_archs){
    FILE *fp;       // puntatore al file
    arch *arc;      // vettore di struct temporaneo per gli archi
    vert *ver;      // vettore di struct temporaneo per i vertici
    int i;
    fp=fopen("file_lab6/grafo.txt","r");
    if(fp==NULL){           // file non torvato o non aperto correttamente
        printf("File non trovato!");
        *archi=NULL;        // assegno null per segnalare l'errore al programma chiamante
        *verts=NULL;
        return;
    }
    fscanf(fp,"%d%d",n_vert,n_archs);           // acquisizione del numero di archi e di vertici
    arc=(arch *)malloc(*n_archs * sizeof(arch));  // allocazione dinamica del vettore degli archi
    ver=(vert *)malloc(*n_vert * sizeof(vert));   // allocazione dinamica del vettore dei vertici
    for(i=0;i<*n_vert;i++)                             // assegnazione dei vertici
        ver[i].val=i;
    for (i = 0; i < *n_archs; i++){     // acquisizione degli archi
        fscanf(fp,"%d %d",&arc[i].first,&arc[i].second);
    }
    fclose(fp);             // chiusura del file
    *archi=arc;             // restituisco l'inidirizzo di memoria del vettore di archi
    *verts=ver;             // restituisco l'indirizzo di memoria del vettore di vertici
}
int BinSearch(vert *sol, int l, int r, int key){

    /* è possibile utilizzare la ricerca binaria poichè l'algoritmo di calcolo del powerset con il
     * meccanismo delle combinazioni ripetute genera dei blocchi i cui elementi interni sono ordinati
     * se l'insieme di partenza è ordinato. In questo caso è noto il numero N di vertici, ed è noto
     * che assumono valori da 0 a N-1; vengono generati i valori dei vertici con un ciclo for, quindi
     * secondo l'ordine usuale dei numeri interi.
     */

    int m=(r+l)/2;          // calcolo l'indice del valore di mezzo
    if(r<l)                // condizione di terminazione: non tovato
        return 0;
    if(key==sol[m].val)     // condizione di terminazione: key trovato
        return 1;
    else if(key<sol[m].val)                // la chiave cercata è più piccola del valore di mezzo
        BinSearch(sol,l,m-1,key);       // prosecuzione ricorsiva nel sottovettore sx
    else if(key>sol[m].val)               // la chiave cercata è più grande del valore di mezzo
        BinSearch(sol,m+1,r,key);       // prosecuzione ricorsiva nel sottovettore dx
}
int CheckSol(arch *archi, int n_arc, vert *sol, int n_act){

    /* Funzione di controllo di validità della soluzione generata dal powerset.
     * una soluzione è valida se presi ad uno ad uno tutti gli archi almeno uno dei due vertici
     * è contenuto nella soluzione. Se si trova almeno un arco i cui vertici non sono contenuti
     * nell asoluzione, si nega il quantificatore esistenziale "per ogni" e quindi la soluzione
     * è da scartare.
     */

    int i,flag_l=0;          // flag_l tiene traccia del fatto che almeno un vertice sia contenuto in sol
    for(i=0; i<n_arc;){      // per tutti gli archi
        if(BinSearch(sol,0,n_act,archi[i].first)==1)    // controllo che il primo vertice sia dentro la soluzione
            flag_l=1;       // se viene trovato, aggiorno il flag relativo all'arco in considerazione
        if(flag_l==0 && BinSearch(sol,0,n_act,archi[i].second)==1)  // se non si è aggiornato il flag dell'arco, controllo il secondo vertice
            flag_l=1;       // se viene trovato, aggiorno il flag relativo all'arco in considerazione
        if(flag_l==1)       // se il flag dell'arco considerato si è aggiornato, allora uno dei due vertici è nella soluzione
            i++;            // aumento il contatore (passo al prossimo arco)
        else
            return 0;       // altrimenti ho trovato almeno un arco i cui vertici non sono in sol
        flag_l=0;           // resetto il flag dell'arco per il successivo
    }
    return 1;               // se il ciclo è terminato tutti gli archi hanno almeno un vertice in sol
}
void powerset_R(arch *archi, vert *verts, int n_arc, int n_vert, vert *sol,int n_act, int pos, int start){

    /* Funzione ricorsiva del powerset.
     * Essa sceglie il valore di indice start tra quelli proposti, lo assegna all'indice pos della soluzione
     * e chiamo ricorsivamente la prossima istanza che scegliera un altro valore (in indice start+1) che lo attrubuirà
     * all'indice pos+1 della soluzione. Questa funzione ricorsiva è contenuta all'interno di un ciclo iterativo
     * per la scelta crescente della dimensione del vettore sol.
     */

    int i;
    if (pos >= n_act){          // condizione di terminazione: raggiunta la dimensione di sol voluta
        if(CheckSol(archi,n_arc,sol,n_act)==1){     // controllo che la soluzione generata sia valida
            printf("{ ");                   // in caso affermativo stampo la soluzione corrente
            for (i = 0; i < n_act; i++)
                printf("%d ", sol[i].val);
            printf("}\n");
        }
        return;
    }
    for (i = start; i < n_vert; i++){       // discesa ricorsiva
        sol[pos].val= verts[i].val;         // scelta del valore della sol da mettere in pos
        powerset_R(archi,verts, n_arc, n_vert, sol, n_act, pos+1, i+1);     // chiamata ricorsiva: pos+1, start=i+1
    }
}
void powerset(arch *archi, vert *verts, int n_arc, int n_vert){
    /* Funzione wrapper del powerset.
     * si dichiara e alloca il vettore soluzione e si itera la dimensione di tale vettore.
     * La funzione wrapper del powerset dovrebbe anche gestire il caso dell'insieme vuoto, ma esso non fa sicuramente
     * parte del vertex cover, quindi non viene stampato.
     */
    /* *archi e *verts sono gli insiemi necessari per stampare e controllare le soluzioni del powerset.
     * In particolare, *verts è il vettore dei vertici sul quale opererà il powerset,
     * *archi è il vettore di archi che verrà passato alla funzione di controllo della validità della
     * soluzione generata.
     *
     * n_verts è il numero di elementi del vettore di archi
     * */
    vert *sol=NULL;          /* il vettore soluzione*/
    int n_act;          /* n_act è la dimensione della soluzione*/
    for(n_act=1;n_act <= n_vert;n_act++){       // ciclo iterativo sulla dimensione di sol
        sol=(vert *)malloc(n_act* sizeof(arch)); /* allocazione del vettore soluzione in base alla dimensione (crescente) */
        powerset_R(archi,verts,n_arc,n_vert,sol,n_act,0,0);   /* chiamata della funzione ricorsiva*/
        free(sol);                          /* deallocazione della memoria del vettore soluzione avente dimensione n */
    }
}
int main(){
    arch *archi=NULL;               // vettore (di strutture) degli archi
    vert *verts=NULL;               // vettore (di strutture) dei vertici
    int n_vert, n_archs,i;          // numero di vertici, numero di archi
    
    // Acquisizione da file
    LoadFile(&archi,&verts,&n_vert,&n_archs);
    if(archi==NULL)                 // se NULL si è verificato un errore durante la lettura del file
        return EXIT_FAILURE;

    printf("Vertici:\n");   // stampa dei vertici
    for(i=0;i<n_vert;i++)
        printf("%d ",verts[i].val);
    printf("\nArchi:\n");   // stampa degli archi letti
    for (i = 0; i < n_archs; i++)
        printf("%d %d \n",archi[i].first,archi[i].second);

    printf("VERTEX COVER:\n");
    powerset(archi,verts,n_archs,n_vert);   // genereazioen del vertex cover tramite powerset di combinazioni ripetute
    return 0;
}