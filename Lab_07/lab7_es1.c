//
// Created by Rosario on 28/11/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define n_gem 4

typedef enum{
    zaf, rub, top, sme
}type;
typedef struct{
    char *name;
    int tot;
}gem;
typedef struct{
    gem gem1;
    gem gem2;
    type t1, t2;
}Choise;
void ScanCollect(gem *col, FILE *fp){
    col[zaf].name=(char *)malloc(sizeof(char));
    strcpy(col[zaf].name,"Z");
    fscanf(fp,"%d",&col[zaf].tot);

    col[rub].name=(char *)malloc(sizeof(char));
    strcpy(col[rub].name,"R");
    fscanf(fp,"%d",&col[rub].tot);

    col[top].name=(char *)malloc(sizeof(char));
    strcpy(col[top].name,"T");
    fscanf(fp,"%d",&col[top].tot);

    col[sme].name=(char *)malloc(sizeof(char));
    strcpy(col[sme].name,"S");
    fscanf(fp,"%d",&col[sme].tot);
}
void PrintCollect(gem *col, int index){
    printf("\nCollezione %d:\n"
           "\tzaffiri: %d"
           "\trubini: %d"
           "\ttopazi: %d"
           "\tsmeraldi: %d",index,col[zaf].tot,col[rub].tot,col[top].tot,col[sme].tot);
}
void FreeCollect(gem *col){
    int i;
    for(i=0;i<n_gem;i++)
        free(col[i].name);
}
type Select(char *name){
    if(strcmp(name,"Z")==0)
        return zaf;
    else if(strcmp(name,"R")==0)
        return rub;
    else if(strcmp(name,"T")==0)
        return top;
    else if(strcmp(name,"S")==0)
        return sme;
}
Choise NextGem(gem *col,gem prec){
    Choise t;
    switch (Select(prec.name)){
        case zaf:
            t.gem1=col[zaf];
            t.t1=zaf;
            t.gem2=col[rub];
            t.t2=rub;
            return t;
        case rub:
            t.gem1=col[sme];
            t.t1=sme;
            t.gem2=col[top];
            t.t2=top;
            return t;
        case top:
            t.gem1=col[zaf];
            t.t1=zaf;
            t.gem2=col[rub];
            t.t2=rub;
            return t;
        case sme:
            t.gem1=col[sme];
            t.t1=sme;
            t.gem2=col[top];
            t.t2=top;
            return t;
    }
}
int LungGreater(int l1, int l2){
    if(l1>l2)
        return 1;
    else return 0;
}
int CheckNextGem(gem *col, gem prec){
    Choise t=NextGem(col,prec);
    t.gem1.tot--;
    t.gem2.tot--;
    if(t.gem1.tot<0 && t.gem2.tot<0)
        return 0;
    else return 1;
}
int Pruning(gem *col, gem act, int lung_t, int lung_abs){
    if(!(CheckNextGem(col,act))==0)     // se scegliendo non arrivo alla condizione di terminazione, la prendo
        return 1;
    else if((CheckNextGem(col,act))==0 && lung_t+1>lung_abs)
        return 1;
    else
        return 0;
}
void FindLenght(gem *col, int lung_t, int *lung_abs, gem prec){
    Choise ch;
    // condizione di terminazione: ho esaurito le scelte possibili sulla prossima gemma
    if(CheckNextGem(col, prec)==0){
        if(LungGreater(lung_t,*lung_abs)==1){
            *lung_abs=lung_t;
            return;
        } else return;
    }

    // se il numero di zaffiri e smeraldi si è esaurito, allora la collana prosegue con un'alternanza di top e rub
    if(col[sme].tot==0 && col[zaf].tot==0){
        while(col[rub].tot==0 || col[top].tot==0){
            if(strcmp(prec.name,"T")==0){
                lung_t++;
                col[rub].tot--;
                strcpy(prec.name,"R");
            } else if(strcmp(prec.name,"R")==0){
                lung_t++;
                col[top].tot--;
                strcpy(prec.name,"T");
            }else if(strcmp(prec.name,"T")!=0 && strcmp(prec.name,"R")!=0){
                if(col[rub].tot>=col[top].tot){
                    col[rub].tot--;
                    strcpy(prec.name,"R");
                }else if(col[top].tot>col[rub].tot){
                    col[top].tot--;
                    strcpy(prec.name,"T");
                }
            }
        }
        return;
    }

    // scelgo la prossima gemma
    ch = NextGem(col,prec);
    // ricorro con la prima gemma
    if(ch.gem1.tot>0 && /*lung_t+1>=*lung_abs*/ Pruning(col,col[ch.t1],lung_t,*lung_abs)){
        col[ch.t1].tot--;
        FindLenght(col, lung_t+1, lung_abs, col[ch.t1]);
        col[ch.t1].tot++;
    }
    // ricorro con la seconda gemma
    if(ch.gem2.tot>0 && /*lung_t+1>=*lung_abs*/Pruning(col,col[ch.t2],lung_t,*lung_abs)){
        col[ch.t2].tot--;
        FindLenght(col, lung_t+1, lung_abs, col[ch.t2]);
        col[ch.t2].tot++;
    }

}
void FindMaxLenght(gem *col,int *lung_abs){
    int i;
    // inserisco ognuna delle pietre per prima e genero n_gem alberi binari
    for(i=0;i<n_gem;i++){
        col[i].tot--;
        FindLenght(col, 1, lung_abs, col[i]);
        col[i].tot++;
    }
}
void LoadFile(){
    FILE *fp;
    char f_name[101];
    gem *col=(gem *)malloc(n_gem* sizeof(gem));
    int rip,i,lung_abs=-1;

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
        ScanCollect(col,fp);   // acquisizione della collezione
        PrintCollect(col,i+1);      // stampa della collezione
        FindMaxLenght(col,&lung_abs);       // determino la lunghezza massima
        printf("\nLunghezza massima: %d\n",lung_abs);
        lung_abs=-1;
        FreeCollect(col);
    }
}
int main(){
    LoadFile();
    return 0;
}