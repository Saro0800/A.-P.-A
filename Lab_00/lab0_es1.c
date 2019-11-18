//
// Created by Rosario on 17/11/2019.
//

#include <stdio.h>
#include <stdlib.h>

#define N 10

int FindMaxLenght(int v[]);
void Print(int v[], int l);

int main(){
    int V[N], lung;
    //Caricamento del vettore valore per valore
    for(int i=0;i<N;i++){
        printf("V[%d]: ",i);
        scanf("%d",&V[i]);
    }

    //Passo il vettore caricato alla funzione di ricerca della stringa di lunghezza massima
    lung=FindMaxLenght(V);printf("\n");

    //Controlli sulla lunghezza massima
    if(lung<=0)
        printf("\nE' stata inserita una sequenza di %d valori nulli.\n",N);
    else if(lung==N) {
        printf("\nLa sequenza di lunghezza massima coincide con il vettore stesso.\nNon e' stato inserito alcuno '0'.\n");
        Print(V, lung);
    }
    else
        Print(V, lung);


    return 0;
}

int FindMaxLenght(int v[]){
    int i,max=-1,temp=0;
    /*  -max: variabile in cui sarà salvata la lunghezza massima
        -temp: variabile in cui viene salvata la lunghezza della temporanea sequenza
        se alla fine di ogni sequenza continua di numeri non nulli temp > max, allora max = temp*/
    for (i=0;i<N;i++) {
        if(v[i]!=0)
            temp++;
        else{
            if(temp>max)
                max=temp;
            temp=0;
        }
    }
    /*  Se non viene inserito alcuno 0, al termine del ciclo for si avrà:
            -temp=N
            -max=-1, non è mai stato aggiornato poichè non si è ncontrato alcuno 0
        Per far fronte a questo problema basta controllare che al termine del ciclo for temp sia =N

        //ATTENZIONE: Il controllo non può essere fatto sul valore assunto da max, in quanto in caso di sequenza nulla
                      il valore di max sarà sempre -1, ma non vi sarà alcun sottovettore valido
    */
    if(temp==N)
        max=temp;
    //fine della funzione. viene restituita la lunghezza massima
    return max;
}

void Print(int v[], int l){
    int i=0,cont=0;
    if(l==N){
        for(int i=0;i<N;i++){
            printf("%d ",v[i]);
        }
    }
    while (i<N){
        if(v[i]!=0)
            cont++;
        else if(v[i]==0 && cont==l){
            for(int j=l; j>0; j--){
                printf("%d ",v[i-j]);
            }
            printf("\n");
        }
        else if(v[i]==0 && cont!=l)
            cont=0;
        i++;
    }
}