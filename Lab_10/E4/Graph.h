//
// Created by s257911 on 15/01/2020.
//

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include "ST.h"

typedef struct edge{ int a; int b; int wt; } Edge;      //struttura per ogni arco
typedef struct graph* Graph;                            // handle del grafo

Graph GRAPHinit(int V);
void GRAPHfree(Graph G);
Graph GRAPHload(FILE *fin);
void GRAPHstore(Graph G, FILE *fout);
void GRAPHinsertE(Graph G, int id1, int id2, int wt);
void GRAPHedges(Graph G, Edge *a);
void GRAPHprintedges(Graph G, FILE *fout);
void GRAPHlistcreate(Graph G);
int GRAPHcomplete(Graph G, char **vet, int dim, int strategy);
int GRAPHlistnull(Graph G);

#endif //GRAPH_H
