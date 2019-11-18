//
// Created by rosar on 17/11/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_c 30
#define max_t 1000

typedef struct{
    char cod[max_c];
    char part[max_c];
    char dest[max_c];
    int data;
    char o_par[max_c];
    char o_arr[max_c];
    int rit;
}TRT;
typedef enum{
    r_date, r_partenza, r_capolinea, r_ritardo, r_ritardo_tot, r_fine, r_err
}comando_e;

int LoadFile(TRT tratte[max_t],int *dim){
    FILE *fp;
    int rig, gg,mm,aa;
    char data[10];
    fp=fopen("file_lab2\\corse.txt","r");
    if(fp==NULL)
        return -1;
    fscanf(fp,"%d",&rig);
    *dim=rig;
    for (int i = 0; i < rig; i++){
        fscanf(fp,"%s %s %s %s %s %s %d",tratte[i].cod,tratte[i].part,tratte[i].dest,data,tratte[i].o_par,tratte[i].o_arr,&tratte[i].rit);
        sscanf(data,"%d/%d/%d",&gg,&mm,&aa);
        tratte[i].data=(aa*10000)+(mm*100)+gg;
    }

    fclose(fp);
    return 0;
}
comando_e LeggiComando(){
    char cmd[max_c];
    printf("\n<date> <partenza> <capolinea> <ritardo> <ritardo_tot> <fine>\nComando: ");
    scanf("%s",cmd);
    if(strcmp(cmd,"date")==0)
        return r_date;
    else if(strcmp(cmd,"partenza")==0)
        return r_partenza;
    else if(strcmp(cmd,"capolinea")==0)
        return r_capolinea;
    else if(strcmp(cmd,"ritardo")==0)
        return r_ritardo;
    else if(strcmp(cmd,"ritardo_tot")==0)
        return r_ritardo_tot;
    else if(strcmp(cmd,"fine")==0)
        return r_fine;
    else
        return r_err;
}
void StampaTratta(TRT tratte[max_t],int i){
    int gg, mm, aa;
    printf("%s %s %s ",tratte[i].cod,tratte[i].part,tratte[i].dest);
    aa=tratte[i].data/10000;
    mm=(tratte[i].data-aa*10000)/100;
    gg=(tratte[i].data-aa*10000-mm*100);
    printf("%d/%d/%d %s %s %d\n",gg,mm,aa,tratte[i].o_par,tratte[i].o_arr,tratte[i].rit);
}
void Date_o_Rit(TRT tratte[max_t],int dim,int data1, int data2,comando_e c){
    if(c==r_date) {
        for (int i = 0; i < dim; i++) {
            if (tratte[i].data >= data1 && tratte[i].data <= data2)
                StampaTratta(tratte, i);
        }
    }else if(c==r_ritardo){
        for (int i = 0; i < dim; i++) {
            if (tratte[i].data >= data1 && tratte[i].data <= data2 && tratte[i].rit!=0)
                StampaTratta(tratte, i);
        }
    }
}
void Part_o_Cap(TRT tratte[max_t],int dim,char ferm[max_c],comando_e c){
    int cont=0;
    if(c==r_partenza){
        for (int i = 0; i < dim; i++) {
            if (strcmp(tratte[i].part,ferm)==0) {
                StampaTratta(tratte, i);
                cont++;
            }
        }
        if(cont==0)
            printf("Fermata di partenza non trovata!\n");
    }
    if(c==r_capolinea){
        for (int i = 0; i < dim; i++) {
            if (strcmp(tratte[i].dest,ferm)==0) {
                StampaTratta(tratte, i);
                cont++;
            }
        }
        if(cont==0)
            printf("Fermata capolinea non trovata!\n");
    }
}
void Ritardo_tot(TRT tratte[max_t],int dim,char cod[max_c]){
    int rit=0;
    for (int i = 0; i < dim; i++){
        if(strcmp(tratte[i].cod,cod)==0 && tratte[i].rit!=0)
            rit+=tratte[i].rit;
    }
    printf("Fermata cod.: %s Ritardo: %d",cod,rit);
}
int SelectDati(TRT tratte[max_t], int dim, comando_e c) {
    int continua=1;
    switch (c) {
        case r_date:{
            char data[max_c];
            int gg,mm,aa;
            int data1, data2;
            printf("Data 1 <gg/mm/aaaa>: ");
            scanf("%s",data);
            sscanf(data,"%d/%d/%d",&gg,&mm,&aa);
            data1=(aa*10000)+(mm*100)+gg;
            printf("Data 2 <gg/mm/aaaa>:  ");
            scanf("%s",data);
            sscanf(data,"%d/%d/%d",&gg,&mm,&aa);
            data2=(aa*10000)+(mm*100)+gg;
            Date_o_Rit(tratte,dim,data1,data2,c);
            break;
        }
        case r_partenza:{
            char ferm[max_c];
            printf("Partenza: ");
            scanf("%s",ferm);
            Part_o_Cap(tratte,dim,ferm,c);
            break;
        }
        case r_capolinea:{
            char ferm[max_c];
            printf("Capolinea: ");
            scanf("%s",ferm);
            Part_o_Cap(tratte,dim,ferm,c);
            break;
        }
        case r_ritardo:{
            char data[max_c];
            int gg,mm,aa;
            int data1, data2;
            printf("Data 1 <gg/mm/aaaa>: ");
            scanf("%s",data);
            sscanf(data,"%d/%d/%d",&gg,&mm,&aa);
            data1=(aa*10000)+(mm*100)+gg;
            printf("Data 2 <gg/mm/aaaa>:  ");
            scanf("%s",data);
            sscanf(data,"%d/%d/%d",&gg,&mm,&aa);
            data2=(aa*10000)+(mm*100)+gg;
            Date_o_Rit(tratte,dim,data1,data2,c);
            break;
        }
        case r_ritardo_tot:{
            char cod[max_c];
            printf("codice tratta: ");
            scanf("%s",cod);
            Ritardo_tot(tratte,dim,cod);
            break;
        }
        case r_fine:
            continua = 0;
            break;
        default:{
            printf("Comando errato!");
            break;
        }
    }
    return continua;
}

int main() {
    TRT tratte[max_t];
    int dim, signal, continua = 1;
    comando_e c;

    //  F caricamento del file
    signal = LoadFile(tratte, &dim);
    if (signal == -1) {
        printf("File non trovato.\n");
        return (EXIT_FAILURE);
    }

    while (continua) {
        c = LeggiComando();
        continua = SelectDati(tratte, dim, c);
    }

    return 0;
}