//
// Created by rosar on 17/11/2019.
//

#include <stdio.h>

#define size 7     // Dimensione del vettore di interi

typedef struct{
    int val;
    int occ;
}major;

major majorityRecursive(int *a, int left, int right) {
    major mag,sx,dx;
    int middle=left+(right-left)/2, N=right-left+1;
    // Condizione di terminazione
    if (right <= left) {
        mag.val = a[left];
        mag.occ = 0;
        return mag;
    }

    // Discesa ricorsiva
    sx=majorityRecursive(a,left,middle);
    dx=majorityRecursive(a,middle+1,right);

    // Conto le occorrenze di sx
    for(int i=left; i<=right; i++)
        sx.occ+=(a[i]==sx.val)? 1:0;

    // Conto le occorrenze di dx
    for(int i=left; i<=right; i++)
        dx.occ+=(a[i]==dx.val)? 1:0;

    // Condizioni di verifica del maggioritario
    if(sx.val==dx.val && (sx.occ+dx.occ)>(N/2)) {// Se sono uguali ritorno uno diloro
        sx.occ=0;
        return sx;
    }
    if(sx.occ>dx.occ && sx.occ>(N/2)) {   // Maggioritario di sinistra ha più occorrenze
        sx.occ = 0;
        return sx;
    }
    if(dx.occ>sx.occ && dx.occ>(N/2)) {   // Maggioritario di destra ha più occorrenze
        dx.occ = 0;
        return dx;
    }

    // Se nessuna delle precedenti condizioni è soddisfatta, allora non esiste un maggioritario
    mag.val=-1;
    mag.occ=0;
    return mag;
}

int majority( int *a, int N){
    int left=0, right=N-1;
    major mag;
    mag=majorityRecursive(a,left,right);
    return mag.val;
}

int main() {
    int vet[size]={3, 3, 9, 4, 3, 5, 3},mag;
    mag=majority(vet, size);
    printf("Il maggioritario e': %d\n",mag);
}