//
// Created by s257911 on 15/01/2020.
//

#ifndef ST_H
#define ST_H

#include <stdio.h>
#include <stdlib.h>

#define lenght 31

typedef struct st* ST;

ST STinit(int V);
void STfree(ST tab);
void STinsert(ST tab, char *id_elab, char *id_rete);
int STcount(ST tab);
void STrealloc(ST tab);
void STsort(ST tab);
int STsearch(ST tab, char *id_elab);
char *STsearchByIndex(ST tab, int i);


#endif //ST_H
