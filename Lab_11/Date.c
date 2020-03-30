//
// Created by s257911 on 19/01/2020.
//

#include "Date.h"
#include <stdlib.h>
#include <stdio.h>

static int DATEconvert(Date d){
    int t = d.aaaa*10000000 + d.mm*1000 + d.gg;
    return t;
}

int DATEeq(Date D1, Date D2){
    int d1 = DATEconvert(D1);
    int d2 = DATEconvert(D2);
    if(d1==d2)
        return 1;
    return 0;
}

int DATEless(Date D1, Date D2){
    int d1 = DATEconvert(D1);
    int d2 = DATEconvert(D2);
    if(d1<d2)
        return 1;
    return 0;
}

int Dategrt(Date D1, Date D2){
    int d1 = DATEconvert(D1);
    int d2 = DATEconvert(D2);
    if(d1>d2)
        return 1;
    return 0;
}

void DatePrint(Date d){
    fprintf(stdout,"%d/%d/%d ",d.aaaa,d.mm,d.gg);
}
