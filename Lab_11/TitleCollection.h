//
// Created by s257911 on 19/01/2020.
//

#ifndef TITLECOLLECTION_H
#define TITLECOLLECTION_H

#include "Title.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct titlecollection* TitleList;

TitleList TITLE_LISTinit();
void TITLE_LISTfree(TitleList t);
int TITLE_LISTvoid(TitleList t);
void TITLE_LISTupdate(TitleList Titles, FILE *fp);
void TITLE_LISTload(TitleList Titles, FILE *fp);
void TITLE_LISTprint(TitleList Titles);
void TITLE_LISTsearch(TitleList Titles);
void TITLE_LISTsearchquot(TitleList Titles);
void TITLE_LISTsearchintervalquot(TitleList Titles, int n);
void TITLE_LISTbalanceBST(TitleList Titles);


#endif //TITLECOLLECTION_H
