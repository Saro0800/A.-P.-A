//
// Created by s257911 on 19/01/2020.
//

#include "QuotationCollection.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct BSTnode* link;
struct BSTnode {
    Quotation quot;
    link l;
    link r;
    int cont;
};
struct BST {
    link root;
    link z;
};

QuotBST QuotBSTinit(){
    QuotBST t = malloc(sizeof *t);
    t->z=NULL;
    t->root=t->z;
    return t;
}

static void QuotBSTfreeR(link head, link z){
    if(head==z)
        return;
    QuotBSTfreeR(head->l,z);
    QuotBSTfreeR(head->r, z);
    free(head);
}

void QuotBSTfree(QuotBST bst){
    QuotBSTfreeR(bst->root, bst->z);
    free(bst->z);
}

link QUOTnew(Quotation x, link z){
    link temp = malloc(sizeof *temp);
    temp->cont = 1;
    temp->quot = x;
    temp->l=z;
    temp->r=z;
    return temp;
}


static link QuotBSTinsertR(link h, link z, Quotation x) {
    if (h == z)
        return QUOTnew(x, z);
    if(DATEeq(h->quot.date,x.date))
        h->quot = QUOTupdate(h->quot,x);
    else if (DATEless(x.date,h->quot.date)){
        h->l = QuotBSTinsertR(h->l, z, x);
        h->cont = 1 + ((h->l==z)? 0:h->l->cont) + ((h->r==z)? 0:h->r->cont);
    }
    else{
        h->r = QuotBSTinsertR(h->r, z, x);
        h->cont = 1 + ((h->l==z)? 0:h->l->cont) + ((h->r==z)? 0:h->r->cont);
    }

    return h;
}

QuotBST QuotBSTread(FILE *fp){
    QuotBST temp = QuotBSTinit();
    //1- leggo la data, ora, valore e quantità
    //inserisco nel BST
    Quotation q;
    Hour h;
    int num;

    //lettura della quantià di operazioni
    fscanf(fp,"%d",&num);

    for(int i=0; i<num; i++){
        fscanf(fp,"%d/%d/%d %d:%d %f %d\n",&q.date.aaaa,&q.date.mm,&q.date.gg,
                &h.h,&h.m,&q.val,&q.quant);
        q.val = q.val * (float)q.quant;
        q.q = q.val /(float)q.quant;
        temp->root = QuotBSTinsertR(temp->root,temp->z,q);
    }

    return temp;
}

Date QuotBSTmindateR(link root, link z){
    if(root->l == z)
        return root->quot.date;

    return QuotBSTmindateR(root->l, z);
}

Date QuotBSTmindate(QuotBST bst){
    return QuotBSTmindateR(bst->root,bst->z);
}

Date QuotBSTmaxdateR(link root, link z){
    if(root->r == z)
        return root->quot.date;

    return QuotBSTmaxdateR(root->r, z);
}

Date QuotBSTmaxdate(QuotBST bst){
    return QuotBSTmaxdateR(bst->root,bst->z);
}

static void QuotBSTprintR(link root, link z){
    if(root==z)
        return;
    //ricorro a sx
    QuotBSTprintR(root->l,z);
    //stampo il nodo
    QUOTprint(root->quot);
    //ricorro a dx
    QuotBSTprintR(root->r,z);
}

void QuotBSTprint(QuotBST bst){
    QuotBSTprintR(bst->root, bst->z);
}

static void QuotBSTupdateinsert(QuotBST dest, Quotation x){
    dest->root = QuotBSTinsertR(dest->root, dest->z, x);
}

static void QuotBSTupdateR(link root, link z, QuotBST dest){
    //condizione di terminazione
    if(root==z)
        return;
    QuotBSTupdateinsert(dest, root->quot);
    QuotBSTupdateR(root->l, z, dest);
    QuotBSTupdateR(root->r, z, dest);
}

void QuotBSTupdate(QuotBST dest, QuotBST new){
    QuotBSTupdateR(new->root, new->z, dest);
}

static void QuotBSTsearchR(link root, link z, Date d){
    if(root==z){
        fprintf(stdout,"Quotazione non trovata per la data inserita.\n");
        return;
    }

    if(DATEeq(d,root->quot.date))
        QUOTprint(root->quot);
    else if(DATEless(d,root->quot.date))
        QuotBSTsearchR(root->l, z, d);
    else
        QuotBSTsearchR(root->r, z, d);
    return;
}

void QuotBSTsearch(QuotBST bst, Date d){
    QuotBSTsearchR(bst->root,bst->z,d);
}

static void QuotBSTsearchintervallR(link root, link z, Date d1, Date d2, Quotation *min, Quotation *max){
    if(root==z || DATEless(root->quot.date,d1) || Dategrt(root->quot.date,d2))
        return;

    if(QUOTless(root->quot,*min))
        *min=root->quot;
    if(QUOTgrt(root->quot,*max))
        *max=root->quot;
    QuotBSTsearchintervallR(root->l,z,d1,d2,min,max);
    QuotBSTsearchintervallR(root->r,z,d1,d2,min,max);
}

void QuotBSTsearchintervall(QuotBST bst, Date d1, Date d2){
    Quotation min, max;
    min.q=INT_MAX;
    max.q=INT_MIN;
    QuotBSTsearchintervallR(bst->root,bst->z,d1,d2,&min, &max);
    fprintf(stdout,"Quotazione minima: ");
    QUOTprint(min);
    fprintf(stdout,"Quotazione massima: ");
    QUOTprint(max);
}

static  void BSTmaxlenght(link head, link z, int *max, int m){
    //condizione di terminazione
    if(head == z){
        if(m > *max)
            *max = m;
        return;
    }

    BSTmaxlenght(head->l, z, max, m+1);
    BSTmaxlenght(head->r, z, max, m+1);
}

static void BSTminlenght(link head, link z, int *min, int m){
    //condizione di terminazione
    if(head == z){
        if(m < *min)
            *min = m;
        return;
    }

    BSTminlenght(head->l, z, min, m+1);
    BSTminlenght(head->r, z, min, m+1);
}

link rotR(link h){
    link x = h->l;
    h->l = x->r;
    x->r = h;
    x->cont = h->cont;
    h->cont = 1;
    h->cont += (h->l) ? h->l->cont : 0;
    h->cont += (h->r) ? h->r->cont : 0;
    return x;
}


link rotL(link h){
    link x = h->r;
    h->r = x->l;
    x->l = h;
    x->cont = h->cont;
    h->cont = 1;
    h->cont += (h->l) ? h->l->cont : 0;
    h->cont += (h->r) ? h->r->cont : 0;
    return x;
}


static link partR(link h, link z, int r){
    int t = ((h->l!=z) ? h->l->cont : 0);
    if (t>r) {
        if(h->l != z)
            h->l = partR(h->l, z, r);
        h = rotR(h);
    }
    if (t<r) {
        if(h->r != z)
            h->r = partR(h->r, z, r - t - 1);
        h = rotL(h);
    }
    return h;
}

static link balanceR(link h, link z){
    int r;
    if (h == z)
        return z;
    r = (h->cont+1)/2-1;
    h = partR(h, z, r);
    h->l = balanceR(h->l, z);
    h->r = balanceR(h->r, z);
    return h;
}


void BSTbalance(QuotBST bst){
    int min=INT_MAX, max=INT_MIN;
    int min_t=INT_MAX, max_t=INT_MIN;
    float soglia = 3;

    BSTminlenght(bst->root, bst->z, &min, -1);  //calcolo del percorso minimo
    BSTmaxlenght(bst->root, bst->z, &max, -1);  //calcolo del percorso massimo

    printf("Pre bilanciamento:\n\tcammino min:%d\n\tcammino max:%d\n",min,max);

    //poichè il rapporto è calcolato tramite max/min, se min=0 il rapporto sarebbe +inf;
    //in questa condizione è necessario bilanciare
    while(min==0 || ((float)max / (float)min) > soglia){
        bst->root = balanceR(bst->root, bst->z);

        //possibile problema: viene richiesto un bilanciamento che non produce effetti
        //occorre verificare se con il nuovo bilanciamento si migliora il min e il max;
        //se è uguale a prima si interrompe l'esecuzione
        BSTminlenght(bst->root, bst->z, &min_t, -1);
        BSTmaxlenght(bst->root, bst->z, &max_t, -1);
        if(min!=0 && (min_t==min || max_t==max))
            break;
        else{
            min=min_t;
            max=max_t;
        }
    }

    printf("Post bilanciamento:\n\tcammino min:%d\n\tcammino max:%d\n",min,max);
}