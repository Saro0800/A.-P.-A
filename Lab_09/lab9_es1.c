//
// Created by s257911 on 20/12/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struttura di ogni attività
typedef struct{
    int start;
    int end;
}att_s;

//struct wrapper
typedef struct{
    int dim;
    att_s *vett;
}att_w;

int LoadFile(att_w *attArray){
    att_w array;
    int dim;
    FILE *fp;
    char nome[101];

    //acquisizione del nome del file
    fprintf(stdout,"Nome file di input: ");
    fscanf(stdin,"%s",nome);
    if(strstr(nome,".txt")==NULL)
        strcat(nome,".txt");

    fp=fopen(nome,"r");
    if(fp==NULL){
        fprintf(stdout,"Errore di apertura del file.\n");
        return -1;
    }

    fscanf(fp,"%d",&dim);
    array.vett=(att_s *)malloc(dim * sizeof(att_s));
    for (int i = 0; i < dim; i++)
        fscanf(fp,"%d %d",&array.vett[i].start,&array.vett[i].end);

    *attArray=array;
    return dim;
}

void Merge(att_s *A, att_s *B, int l, int q, int r) {
    int i, j, k;
    i = l;
    j = q+1;
    for(k = l; k <= r; k++)
        if (i > q)
            B[k] = A[j++];
        else if (j > r)
            B[k] = A[i++];
        else if (A[i].start<=A[j].start )
            B[k] = A[i++];
        else
            B[k] = A[j++];
    for ( k = l; k <= r; k++ )
        A[k] = B[k];
    return;
}

void MergeSortR(att_s *A, att_s *B, int l, int r) {
    int q;
    if (r <= l)
        return;
    q = (l + r)/2;
    MergeSortR(A, B, l, q);
    MergeSortR(A, B, q+1, r);
    Merge(A, B, l, q, r);
}

void MergeSort(att_s *A, int N) {
    int l=0, r=N-1;
    att_s *B = (att_s *)malloc(N*sizeof(att_s));
    if (B == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    MergeSortR(A, B, l, r);
}

void LISprint(att_s *val, int *P, int i) {
    if (P[i]==-1) {
        printf("%d %d\n", val[i].start,val[i].end);
        return;
    }
    LISprint(val, P, P[i]);
    printf("%d %d\n", val[i].start,val[i].end);
}

void LISDP(att_s *val, int N) {
  int i, j, ris, L[N], P[N], last;
  ris = 0;
  last = 0;
  for (i=0; i<N; i++) {
      L[i] = val[i].end - val[i].start;
      P[i] = -1;
    for (j=0; j<i; j++)
      if (val[i].start>=val[j].end && L[i] < L[j] + val[i].end - val[i].start ) {
          L[i] = val[i].end - val[i].start + L[j];
          P[i] = j;
        }
    if (ris < L[i]) {
      ris = L[i];
      last = i;
    }
  }
  printf("\nOne the best solution is:\n");
  LISprint(val, P, last);
  printf("\n");
  printf("Max time occupied: %d\n", ris);

}


int main(){
    att_w attArray;
    int dim = LoadFile(&attArray);
    if(dim<=0)
        return EXIT_FAILURE;
    fprintf(stdout,"%d activities have been inserted\n",dim);
    fprintf(stdout,"\nACTIVITIES INSERTED:\n");
    for (int i = 0; i < dim; i++)
        printf("%d %d\n",attArray.vett[i].start,attArray.vett[i].end);
    MergeSort(attArray.vett,dim);
    fprintf(stdout,"\nSORTED ACTIVITIES by increasing start time:\n");
    for (int i = 0; i < dim; i++)
        printf("%d %d\n",attArray.vett[i].start,attArray.vett[i].end);
    LISDP(attArray.vett,dim);


    return EXIT_SUCCESS;
}





