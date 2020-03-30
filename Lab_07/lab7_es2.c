//
// Created by Rosario Francesco Cavelli on 30/11/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



#define n_gem 4

typedef enum{
    zaf, rub, top, sme
}type;
typedef struct{
    char *name;
    int tot;
    unsigned int val;
}gem;
typedef struct{
    gem gem1;
    gem gem2;
    type t1, t2;
}Choise;
typedef struct{
    char *seq;
    unsigned int lung;
    unsigned int val;
}Solution;
void ScanCollect(gem *col, FILE *fp, int *lim){
    // attribuzione dei nomi della collana
    col[zaf].name=(char *)malloc(sizeof(char));
    strcpy(col[zaf].name,"Z");
    col[rub].name=(char *)malloc(sizeof(char));
    strcpy(col[rub].name,"R");
    col[top].name=(char *)malloc(sizeof(char));
    strcpy(col[top].name,"T");
    col[sme].name=(char *)malloc(sizeof(char));
    strcpy(col[sme].name,"S");

    // acquisizione delle quantità
    fscanf(fp,"%d %d %d %d ",&col[zaf].tot,&col[rub].tot,&col[top].tot,&col[sme].tot);

    // acquisiszione deli valori
    fscanf(fp,"%d %d %d %d ",&col[zaf].val,&col[rub].val,&col[top].val,&col[sme].val);

    // acquisizione del limite di ripetizioni
    fscanf(fp,"%d",lim);

}
void PrintCollect(gem *col, int index,int lim){
    printf("\nTest #%d:\n"
           "zaffiro: %d [%d], "
           "rubino: %d [%d], "
           "topazio: %d [%d], "
           "smeraldo: %d [%d], "
           "max rip: %d\n",index,col[zaf].tot,col[zaf].val,
                                   col[rub].tot,col[rub].val,
                                   col[top].tot,col[top].val,
                                   col[sme].tot,col[sme].val,lim);
}
void FreeCollect(gem *col){
    int i;
    for(i=0;i<n_gem;i++)
        free(col[i].name);
}
Solution SolSetVoid(int max){
    Solution temp;
    temp.seq=(char *)malloc((max + 1)* sizeof(char));
    strcpy(temp.seq,"\0");
    temp.val=0;
    temp.lung=0;
    return temp;
}
int EmeraldFirst(Solution sol){
    int i;
    int s=0, z=0;
    for(i=0;i<strlen(sol.seq);i++){
        if(sol.seq[i]=='S' && z==0)
            return 1;
        if(sol.seq[i]=='Z' && s==0)
            return 0;
    }
}
int NumSmeZaf(Solution sol){
    int s=0, z=0;
    for(int i=0;i<strlen(sol.seq);i++){
        if(sol.seq[i]=='S')
            s++;
        else if(sol.seq[i]=='Z')
            z++;
        if(z>s)
            return 0;
    }
    return 1;
}
int CheckRip(Solution sol, int max_rip){
    // funzione per il controllo che nessuna pietra si ripeta più del valore consentito
    int col=1;
    //int occ[n_gem]={0};
    char last=sol.seq[0];
    // se una sola delle occorrenza delle pietre è maggiore di max_rip, la soluzione è da scartare
    // viene interrotto anticipatamente il ciclo e viene restituito 0
    for (int i = 1; i < sol.lung; i++) {
        if(sol.seq[i]!=sol.seq[i-1])
            col=1;
        else col++;
        if(col>max_rip)
            return 0;
    }
    // se il ciclo viene terminato, nessuna pietra è presente più di max_rip
    // viene restituito quindi 1
    return 1;
}
int CheckAmaount(int pos, gem *col, Solution sol){
    // funzione per controllare che l'ultima possa effettivamente essere scelta,
    // ovvero se la quantità per quel tipo di pietra sia positiva
    switch (sol.seq[pos]){
        case 'Z':
            if(col[zaf].tot>=0)
                return 1;
            else return 0;
        case 'R':
            if(col[rub].tot>=0)
                return 1;
            else return 0;
        case 'T':
            if(col[top].tot>=0)
                return 1;
            else return 0;
        case 'S':
            if(col[sme].tot>=0)
                return 1;
            else return 0;
    }
}
int CheckSelect(int pos, Solution sol, gem *col, int max_rip){
    // occore controllare (nell'ordine):
    //      1- che sia effettivamente possibile prendere una certa pietre (quant>0)
    //      2- che la sequenza di pietre sia concorde con le regole stabilite
    //      3- che nella soluzione non siano presenti pietre con più ripetizioni del massimo consentito
    //      4- che nella soluzione compaia prima uno smeraldo e poi uno zaffiro
    //      5- che il numero di zaf. sia minore del numero di sme.
    if(pos==0 && sol.seq[pos]=='Z')
        return 0;
    else if(pos==0 && sol.seq[pos]!='Z')
        return 1;
    switch (sol.seq[pos-1]){
        case 'Z':
            if((sol.seq[pos]=='Z' || sol.seq[pos]=='R') && CheckAmaount(pos,col,sol) && CheckRip(sol,max_rip)
                                                        && EmeraldFirst(sol) && NumSmeZaf(sol))
                return 1;
            else return 0;
        case 'S':
            if((sol.seq[pos]=='S' || sol.seq[pos]=='T') && CheckAmaount(pos,col,sol) && CheckRip(sol,max_rip)
                                                        && EmeraldFirst(sol) && NumSmeZaf(sol))
                return 1;
            else return 0;
        case 'R':
            if((sol.seq[pos]=='S' || sol.seq[pos]=='T') && CheckAmaount(pos,col,sol) && CheckRip(sol,max_rip)
                                                        && EmeraldFirst(sol) && NumSmeZaf(sol))
                return 1;
            else return 0;
        case 'T':
            if((sol.seq[pos]=='Z' || sol.seq[pos]=='R') && CheckAmaount(pos,col,sol) && CheckRip(sol,max_rip)
                                                        && EmeraldFirst(sol) && NumSmeZaf(sol))
                return 1;
            else return 0;
    }


}
int NextSelect(int pos, Solution sol, gem *col){
    switch (sol.seq[pos-1]){
        case 'Z':
            if(col[zaf].tot==0 && col[rub].tot==0)
                return 0;
            else return 1;
        case 'S':
            if(col[sme].tot==0 && col[top].tot==0)
                return 0;
            else return 1;
        case 'R':
            if(col[sme].tot==0 && col[top].tot==0)
                return 0;
            else return 1;
        case 'T':
            if(col[zaf].tot==0 && col[rub].tot==0)
                return 0;
            else return 1;
    }

}
void Perm_rip(int pos, char *val, Solution *sol_abs, Solution *sol, gem *col, int max_rip, int k){
    int i;
    if(pos>=k || (pos>0 && NextSelect(pos,*sol,col)==0)){
        // MODIFICA DELLA SOLUZIONE GLOBALE
        if(sol->val>sol_abs->val){
            sol_abs->val=sol->val;
            sol_abs->lung=sol->lung;
            strcpy(sol_abs->seq,sol->seq);
            return;
        } else return;
    }
// PERMUTAZIONI SEMPLICI
    for(i=0;i<n_gem;i++){
        col[i].tot--;
        sol->seq[pos]=val[i];   // aggiorno la sequenza temporanea
        sol->lung++;            // aggiorno la lunghezza temporanea
        sol->val+=col[i].val;   // aggiorno il valore temporaneo
        if(CheckSelect(pos,*sol,col,max_rip))
            Perm_rip(pos+1,val,sol_abs,sol,col,max_rip,k);
        col[i].tot++;
        sol->lung--;
        sol->val-=col[i].val;
    }

}
void FindMaxLenght(gem *col,Solution *sol_abs, int lim){
    // divido il vettore che rappresenta la collezione di pietre in tre vettori
    char *names=(char *)malloc((n_gem + 1)  * sizeof(char));     // uno che rappresenta i caratteri della soluzione
    int i,max=0;
    Solution sol;

    for(i=0;i<n_gem;i++){
        names[i]=col[i].name[0];
        max+=col[i].tot;
    }
    names[i]='\0';
    *sol_abs=SolSetVoid(max);

    //si generano le disposizioni con dimensione variabile
    for(i=max;i>0;i--){
        sol=SolSetVoid(max);
        Perm_rip(0,names,sol_abs,&sol,col,lim,i);
        if(sol_abs->lung>0 && sol_abs->val>0)
          break;
    }
    sol_abs->seq[sol_abs->lung]='\0';
    free(sol.seq);
}
void LoadFile(){
    FILE *fp;
    char f_name[101];
    gem *col=(gem *)malloc(n_gem* sizeof(gem));
    Solution sol_abs;
    int rip,i;
    int max_rip;        // numero massimo di ripetizioni

    // acquisizione nome del file
    printf("Nome file input: ");
    scanf("%s",f_name);
    if(strstr(f_name,".txt")==NULL)
        strcat(f_name,".txt");

    fp=fopen(f_name,"r");
    if(fp==NULL){
        printf("File non trovato!\n");
        return ;
    }
    // acquisisco il numero di collezioni trovate
    fscanf(fp,"%d",&rip);
    // per ogni collezione
    for(i=0;i<rip;i++){
        ScanCollect(col,fp,&max_rip);               // acquisizione della collezione
        PrintCollect(col,i+1,max_rip);       // stampa della collezione
        FindMaxLenght(col,&sol_abs,max_rip);       // determino la lunghezza massima
        printf("Valore ottimo: %d",sol_abs.val);
        printf("\nLunghezza massima: %d\n",sol_abs.lung);
        printf("Sequenza trovata:\t %s\n",sol_abs.seq);
        free(sol_abs.seq);
        sol_abs.lung=-1;
        sol_abs.val=-1;
        FreeCollect(col);
    }
}
int main(){
    clock_t start,end;
    double tempo;
    start=clock();
    LoadFile();
    end=clock();
    tempo=((double)(end-start))/CLOCKS_PER_SEC;
    printf("\n\nTempo di esecuzione: %f s",tempo);
    return 0;
}
