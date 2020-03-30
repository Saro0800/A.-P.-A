//
// Created by s257911 on 15/01/2020.
//

#include <stdio.h>
#include "Graph.h"
#include "ST.h"

typedef struct vert* link;  //alias per il puntatore al prossimo nodo della lista
struct vert{
    char *name;     //nome del vertice
    int index;      //indice del vertice all'interno della tabella di simboli
    int wt;         //peso dell'arco
    link next;      //puntatore al prossimo nodo
};      //struttura del vertice (nodo) della lista di adiacenza

struct graph{
    int V;          //numero di vertici
    int E;          //numero di archi
    int **madj;     //matrice delle adiacenze
    link *head;      //lista delle adiacenze (vettore di liste)
    ST tab;         //tabella di simboli
};

static link* LISTinit(int V){
    link *t;
    t=(link *)malloc(V * sizeof(link));
    for(int i=0; i<V; i++)
        t[i]=NULL;
    return t;
}
static int** MATRIXinit(int r, int c, int val){
    int **matrix, i, j;
    matrix = (int**)malloc(r*(sizeof(int*)));
    for(i=0; i<r; i++)
        matrix[i]=malloc(c* sizeof(int));

    for(i=0; i<r; i++)
        for(j=0; j<c; j++)
            matrix[i][j]=val;

    return matrix;
}
Graph GRAPHinit(int V){
    Graph G;
    G=malloc(sizeof *G);
    G->V=V;     //numero di vertici
    G->E=0;     //numero di archi
    G->head = NULL;  //inizializzazione della lista di adiacenza
    G->madj = MATRIXinit(V, V, -1);     //inizializzazione della matrice di adiacenza
    G->tab = STinit(V);     //inizializzazione della tabella di simboli
    return G;
}

static Edge EDGEcreate(int id1, int id2, int wt){
    Edge t;
    t.a = id1;
    t.b = id2;
    t.wt = wt;
    return t;
}

static void insertE(Graph G, Edge x){
    int i=x.a, j=x.b;
    G->madj[i][j]=x.wt;
}

void GRAPHinsertE(Graph G, int id1, int id2, int wt){
    insertE(G, EDGEcreate(id1, id2, wt));
    insertE(G, EDGEcreate(id2, id1, wt));
}

Graph GRAPHload(FILE *fin){
    //il formato del file non è standard; occorre quindi prima sovrastimare il numero di vertici
    int V;
    int cont=0, wt;
    char s1[lenght], s2[lenght], s3[lenght], s4[lenght];
    Graph G;
    ST tab;
    while(fscanf(fin,"%s %s %s %s %d",s1,s2,s3,s4,&wt)==5)
        cont++;
    V=2*cont;       //sovrastimo il numero di vertici

    //inserisco i vertci nella tabella di simboli
    rewind(fin);
    tab = STinit(V);
    while(fscanf(fin,"%s %s %s %s %d",s1,s2,s3,s4,&wt)==5){
        STinsert(tab, s1, s2);
        STinsert(tab,s3, s4);
    }
    //reallocazione della tabella di simboli
    STrealloc(tab);
    //ordinamento della tabella di simboli
    STsort(tab);
    //aggiorno il numero di vertci del grafo
    V=STcount(tab);

    //inzializzazione del grafo
    G = GRAPHinit(V);
    //salvo la tabella di simboli
    G->tab=tab;

    //rappresentazione del grafo (matrice di adiacenza)
    rewind(fin);
    while(fscanf(fin,"%s %s %s %s %d",s1,s2,s3,s4,&wt)==5){
        int id1 = STsearch(G->tab, s1);
        int id2 = STsearch(G->tab, s3);
        if(id1 >= 0 && id2 >= 0){
            GRAPHinsertE(G,id1,id2,wt);
            G->E++;
        }

    }

    return G;
}

void GRAPHedges(Graph G, Edge *a) {
    int v, w, E = 0;
    for (v = 0; v < G->V; v++)
        for (w = v + 1; w < G->V; w++)
            if (G->madj[v][w] >= 0)
                a[E++] = EDGEcreate(v, w, G->madj[v][w]);
}


void GRAPHstore(Graph G, FILE *fout) { int i; Edge *a;
    a = malloc(G->E * sizeof(Edge));
/*
    for(int i=0; i<G->V; i++) {
        for (int j = 0; j < G->V; j++)
            printf("%d ", G->madj[i][j]);
        printf("\n");
    }
*/
    GRAPHedges(G, a);

    fprintf(fout, "Vertici: %d\n", G->V);
    for (i = 0; i < G->V; i++)
        fprintf(fout, "\t%s\n", STsearchByIndex(G->tab, i));
    fprintf(fout, "Archi: %d\n", G->E);
    for (i = 0; i < G->E; i++)
        fprintf(fout, "\t%s  %s %d\n", STsearchByIndex(G->tab, a[i].a), STsearchByIndex(G->tab, a[i].b), a[i].wt);
}

void GRAPHprintedges(Graph G, FILE *fout){
    /* è richiesto che venga effettuta la stampa dei vertici incidenti per ciscuno dei vertici del grafo.
     * Ogni riga della matrice contiene l'insieme di archi che escono da un vertice. Poichè il grafo è non orientato,
     * ogni arco uscente è anche un arco entrante, quindi è sufficiente elencare i vertci a cui il grafo è adiacente,
     * ovvero gli indici di colonna di ogni riga in cui è presente un valore valido per inidicare un arco di adiacenza.
     *
     * Inoltre l'insieme dei vertici visualizzato è gia ordinato in ordine alfabetico.
     * Questo è dovuto al fatto che la tabella di simboli è stata ordinata alfabeticamente; poichè ogni riga
     * della matrice rappresenta un vettore coinciedente con la tabella di simboli, anche tutte le righe della matrice
     * risultano odinate alfabeticamente.
     */
    for(int i=0; i<G->V; i++){
        fprintf(fout, "%s\n", STsearchByIndex(G->tab, i));
        for(int j=0; j<G->V; j++)
            if(G->madj[i][j]>=0)
                fprintf(fout, "\t%s\n", STsearchByIndex(G->tab, j));
    }
}

static link NewNode(ST tab, int index, int wt, link next){
    link x;
    x=malloc(sizeof *x);    //allocazione del nuovo nodo
    x->index = index;
    x->name = STsearchByIndex(tab, index);
    x->wt = wt;
    x->next = next;
    return x;
}

static void LISTinsert(Graph G, Edge x){
    int a= x.a, b=x.b, wt=x.wt;
    int i;
    //viene effettuata una scansione lineare per inserire l'arco
    //l'inerimento viene effettuato in testa alla lista, così da rendere l'inserimento O(1).
    //in questo modo l'inserimento di un arco è O(V); poichè ogni arco viene letto dalla matrice
    //di adiacenza, la complessità totale dell'operazione è O(V^3)
    for(i=0; i<G->V; i++)
        if(a == G->head[i]->index)
            G->head[i]->next = NewNode(G->tab, b, wt, G->head[i]->next);

}

void GRAPHlistcreate(Graph G){
    int i,j;

    G->head = LISTinit(G->V);

    //creazione dei nuovi nodi principali
    for(i=0; i<G->V; i++)
        G->head[i] = NewNode(G->tab, i, 0, NULL);

    //scansione della matrice di adiacenza e inserimento degli archi nella lista
    for(i=0; i<G->V; i++)
        for(j=0; j<G->V; j++)
            if(G->madj[i][j]>=0)
            LISTinsert(G, EDGEcreate(i,j,G->madj[i][j]));

    for(i=0; i<G->V; i++){
        fprintf(stdout,"%d %s (%d)\n",G->head[i]->index,G->head[i]->name,G->head[i]->wt);
        for(link x=G->head[i]->next; x!=NULL; x=x->next)
            fprintf(stdout,"\t%d %s (%d)\n",x->index,x->name,x->wt);
    }
}

static int EDGEfindmatrix(Graph G, Edge x){
    int a=x.a, b=x.b;
    if(G->madj[a][b]>=0)
        return 1;
    return 0;
}

static int EDGEfindlist(Graph G, Edge x){
    int a=x.a, b=x.b;
    link t;
    //si percorre la lista di adiacenza del vertice a fino a trovare il vertice b
    //se non viene trovato, la lista viene percorsa completamente e t diventerà uguale a NULL
    for(t=G->head[a]; t->index!=b && t!=NULL; t=t->next);
    if(t==NULL)
        return 0;
    return 1;
}

int GRAPHcomplete(Graph G, char **vet, int dim, int strategy){
    int indexes[dim];   //vettore per la conversione delle stringhe in indici dei vertici
    int i,j;

    //conversione da nomi inseriti a indici della tabella di simboli
    for(i=0; i<dim; i++){
        if(STsearch(G->tab, vet[i])!=-1)
            indexes[i]= STsearch(G->tab, vet[i]);
        else{
            printf("%s non presente in elenco\n",vet[i]);
            return -1;
        }
    }

    /* Dato che il grafo è non orientato, per vedere se un sottografo di 'dim' vertici è completo è
     * sufficiente controllare che per il vertice i-esimo esistono 'dim-1-i' archi che lo collegano
     * ai 'dim-1-i' vertici successivamente inseriti.
     */

    for(i=0; i<dim; i++){
        for(j=i+1; j<dim; j++)
            //verifica tramite matrice di adiacenza
            if(strategy==1 && !EDGEfindmatrix(G, EDGEcreate(indexes[i],indexes[j],1))){
                printf("Arco (%s - %s) non esistente.\n",STsearchByIndex(G->tab,indexes[i]),STsearchByIndex(G->tab,indexes[j]));
                return 0;
            }
            //verifica tramite lista di adiacenza
            else if (strategy==2 && !EDGEfindlist(G, EDGEcreate(indexes[i],indexes[j],1))){
                printf("Arco (%s - %s) non esistente.\n",STsearchByIndex(G->tab,indexes[i]),STsearchByIndex(G->tab,indexes[j]));
                return 0;
            }
    }
    return 1;
}

static void GRAPHlistfreeR(link x){
    if(x==NULL)
        return;

    GRAPHlistfreeR(x->next);
    free(x->name);
    free(x);
}
static void GRAPHlistfree(Graph G){
    if(G->head == NULL)
        return;
    for(int i=0; i<G->V; i++)
        GRAPHlistfreeR(G->head[i]);
    free(G->head);
}

void GRAPHfree(Graph G){
    int i;
    for(i=0; i<G->V; i++)
        free(G->madj[i]);
    free(G->madj);
    GRAPHlistfree(G);
    STfree(G->tab);
    free(G);
}

int GRAPHlistnull(Graph G){
    if(G->head==NULL)
        return 1;
    return 0;
}


