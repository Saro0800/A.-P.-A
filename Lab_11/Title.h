//
// Created by s257911 on 19/01/2020.
//

#ifndef TITLE_H
#define TITLE_H

#include "Date.h"
#include "Hour.h"
#include "QuotationCollection.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct title* Title;

Title TITLEnext(Title x);
void TITLEfree(Title x);
Title TITLEnew(Title val, Title next);
int TITLEcodgrt(Title t1, Title t2);
int TITLEcodeq(Title t1, Title t2);
Title TITLEread(FILE* fp);
void TITLEprint(Title x);
void TITLEsetnext(Title dest, Title new);
void TITLEupdate(Title dest, Title new);
void TITLEsearchquot(Title x);
void TITLEsearchquotintervall(Title x, int n);
void TITLEbalanceBST(Title x);



#endif //TITLE_H
