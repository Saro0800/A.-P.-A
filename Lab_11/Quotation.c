//
// Created by s257911 on 19/01/2020.
//

#include "Quotation.h"
#include <stdio.h>
#include <stdlib.h>

Quotation QUOTupdate(Quotation h, Quotation x){
    Quotation temp=h;

    temp.val += x.val;
    temp.quant += x.quant;
    temp.q = temp.val/(float)temp.quant;
    return temp;
}

void QUOTprint(Quotation x){
    fprintf(stdout,"\n\tData: ");
    DatePrint(x.date);
    fprintf(stdout,"%f\n",x.q);
}

int QUOTless(Quotation Q1, Quotation Q2){
    if(Q1.q < Q2.q)
        return 1;
    return 0;
}

int QUOTgrt(Quotation Q1, Quotation Q2){
    if(Q1.q > Q2.q)
        return 1;
    return 0;
}

