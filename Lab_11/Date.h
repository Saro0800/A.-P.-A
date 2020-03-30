//
// Created by rosar on 19/01/2020.
//

#ifndef DATE_H
#define DATE_H

typedef struct{
    int aaaa;
    int mm;
    int gg;
}Date;

int DATEeq(Date D1, Date D2);
int DATEless(Date D1, Date D2);
int Dategrt(Date D1, Date D2);
void DatePrint(Date d);

#endif //DATE_H
