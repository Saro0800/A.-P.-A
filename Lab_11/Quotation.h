//
// Created by s257911 on 19/01/2020.
//

#ifndef QUOTATION_H
#define QUOTATION_H

#include "Date.h"
#include "Hour.h"

typedef struct{
    Date date;      //data della quotazione
    float val;      //valore accumulato
    int quant;      //numero di operazioni
    float q;     //valore della quotazione
}Quotation;

Quotation QUOTupdate(Quotation h, Quotation x);
void QUOTprint(Quotation x);
int QUOTless(Quotation Q1, Quotation Q2);
int QUOTgrt(Quotation Q1, Quotation Q2);

#endif //QUOTATION_H
