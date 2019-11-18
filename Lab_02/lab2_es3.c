//
// Created by rosar on 17/11/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define max_c 5
#define max_seq 20
#define max_word 25

typedef struct{
    char sequenza[max_c];
    int ind;
}Seq;

void getns(char confr[max_c],int start,char word[max_word],int lung){
    int i;
    for(i=0;i<lung;i++){
        confr[i]=tolower(word[start+i]);
    }
    confr[i]='\0';
}

int main() {
    FILE *fp;
    int seq,i,cont=0, parole=0,k, punct=0,ricorrenza=0;
    Seq sequenze[max_seq];
    char word[max_word],riga[200],confr[max_c];

    //Carico le sequenze
    fp=fopen("file_lab2\\sequenze.txt","r");
    if(fp==NULL){
        printf("Errore sequenze.txt");
        return(EXIT_FAILURE);
    }
    //Numero sequenze
    fscanf(fp,"%d",&seq);
    for(i=0; i<seq;i++){
        fscanf(fp,"%s",word);
        for(int j=0;j<strlen(word); j++)
            word[j]=tolower(word[j]);
        strcpy(sequenze[i].sequenza,word);
        sequenze[i].ind=i;
    }
    fclose(fp);

    fp=fopen("file_lab2\\testo.txt","r");
    //Determino il numero di parole
    while(cont<i) {
        rewind(fp);
        parole=0;
        ricorrenza=0;
        printf("La sequenza '%s' e' contenuta in:\n", sequenze[cont].sequenza);
        while (fgets(riga, 200, fp) != NULL) {
            k = 0;
            for (int j = 0; j < strlen(riga); j++) {
                if (isalnum(riga[j])!= 0) {
                    punct = 0;
                    word[k] = tolower(riga[j]);
                    punct = 0;
                    k++;
                } else if (punct == 0 && (isspace(riga[j]) != 0 || ispunct(riga[j]) != 0 || iscntrl(riga[j]) != 0)) {
                    punct = 1;
                    parole++;
                    word[k]='\0';
                    k=0;
                    for (int l=0;l<=abs(strlen(word)-strlen(sequenze[cont].sequenza));l++){
                        getns(confr,l,word,strlen(sequenze[cont].sequenza));
                        if(strcmp(confr,sequenze[cont].sequenza)==0) {
                            printf("posizione %d (%s)\n", parole,word);
                            ricorrenza++;
                            break;
                        }
                    }
                    if(ricorrenza==10)
                        break;
                }
            }
        }
        if(ricorrenza==0)
            printf("nessuna parola\n");
        printf("\n");
        cont++;
    }

    return 0;
}