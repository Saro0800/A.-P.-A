//
// Created by s257911 on 15/01/2020.
//

#include "Graph.h"
#include <string.h>

void CHECKgraphcomplete(Graph G){
    char name[lenght];
    char **vet;
    int bool,strategy;
    vet=(char **)malloc(3 * sizeof(char *));
    //lettura della strategia
    fprintf(stdout,"Ricerca tramite:\n"
                   "\t1- Matrice di adiacenza\n"
                   "\t2- Lista di adiacenza\n"
                   "Strategia: ");
    fscanf(stdin,"%d",&strategy);
    if(strategy==2 && GRAPHlistnull(G)){
        fprintf(stdout,"Lista di adiacenza non ancora creata\n");
        return;
    }

    //lettura degli archi da inserire
    for(int i=0; i<3; i++) {
        printf("Vertice %d: ", i + 1);
        scanf("%s",name);
        vet[i]=strdup(name);
    }

    bool = GRAPHcomplete(G,vet,3,strategy);

    if(bool == 1)
        fprintf(stdout,"\nSottografo completo.\n");
    else if(bool == 0)
        fprintf(stdout,"\nSottografo non completo.\n");
    else if (bool == -1)
        fprintf(stdout, "\nErrore di inserimento dei vertici.\n");

    //deallocazione del vettore di stringhe
    for(int i=0; i<3; i++)
        free(vet[i]);
    free(vet);
}

void RunIstruction(Graph G, int command){
    if(command == 1)
        GRAPHprintedges(G,stdout);
    else if(command == 2)
        CHECKgraphcomplete(G);
    else if(command == 3)
        GRAPHlistcreate(G);
    else
        fprintf(stdout,"Comando inserito non valido!\n");
}

int main(int argc, char **argv) {
    int fine=0, command;
    FILE *fin;
    Graph G;
    if (argc <= 0) {
        printf("Nome non inserito in linea di comando.\n");
        return EXIT_FAILURE;
    }
    if (strstr(argv[0], ".txt") == NULL)
        strcat(argv[0], ".txt");
    fin = fopen("file_lab10/grafo.txt", "r");
    if (fin == NULL){
        printf("File non trovato.\n");
        return EXIT_FAILURE;
    }

    //lettura del grafo
    G = GRAPHload(fin);
    //stampa del grafo
    GRAPHstore(G, stdout);

    while(!fine){
        printf("\nElenco di comanndi:\n"
               "\t1- Genera elenco dei vertici incidenti\n"
               "\t2- Verifica sottografo completo\n"
               "\t3- Genera lista di adiacenza\n"
               "\t4- Fine\n"
               "Comando: ");
        scanf("%d",&command);
        if(command == 4)
            fine = 1;
        else
            RunIstruction(G, command);
    }

    GRAPHfree(G);
    return EXIT_SUCCESS;

}

