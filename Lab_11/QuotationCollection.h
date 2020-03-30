//
// Created by s257911 on 19/01/2020.
//

#ifndef QUOTATIONCOLLECTION_H
#define QUOTATIONCOLLECTION_H

#include "Quotation.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct BST *QuotBST;

QuotBST QuotBSTinit();
void QuotBSTfree(QuotBST bst);
QuotBST QuotBSTread(FILE *fp);
Date QuotBSTmindate(QuotBST bst);
Date QuotBSTmaxdate(QuotBST bst);
void QuotBSTprint(QuotBST bst);
void QuotBSTupdate(QuotBST dest, QuotBST new);
void QuotBSTsearch(QuotBST bst, Date d);
void QuotBSTsearchintervall(QuotBST bst, Date d1, Date d2);
void BSTbalance(QuotBST bst);


#endif //QUOTATIONCOLLECTION_H
