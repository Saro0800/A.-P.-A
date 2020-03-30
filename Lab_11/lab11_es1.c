//
// Created by s257911 on 19/01/2020.
//

#include "TitleCollection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void RunOption(TitleList Titles, int command){
    char name[101];
    FILE *fp;

    switch(command){
        case 1:
            fprintf(stdout,"Nome del file: ");
            fscanf(stdin,"%s",name);
            if(strstr(name,".txt")==NULL)
                strcat(name,".txt");
            fp=fopen(name, "r");
            if(fp==NULL){
                printf("File non trovato.\n");
                return;
            }
            //se la lista è vuota viene inizializzata
            if(TITLE_LISTvoid(Titles))
                TITLE_LISTload(Titles, fp);
                //altrimenti viene aggiornata
            else
                TITLE_LISTupdate(Titles, fp);
            fclose(fp);

            TITLE_LISTprint(Titles);
            break;

        case 2:
            if(TITLE_LISTvoid(Titles))
                fprintf(stdout,"Lista titoli vuota\n");
            else
                TITLE_LISTsearch(Titles);
            break;

        case 3:
            if(TITLE_LISTvoid(Titles))
                fprintf(stdout,"Lista titoli vuota\n");
            else
                TITLE_LISTsearchquot(Titles);
            break;

        case 4:
            if(TITLE_LISTvoid(Titles))
                fprintf(stdout,"Lista titoli vuota\n");
            else
                TITLE_LISTsearchintervalquot(Titles,1);
            break;

        case 5:
            if(TITLE_LISTvoid(Titles))
                fprintf(stdout,"Lista titoli vuota\n");
            else
                TITLE_LISTsearchintervalquot(Titles,2);
            break;

        case 6:
            if(TITLE_LISTvoid(Titles))
                fprintf(stdout,"Lista titoli vuota\n");
            else
                TITLE_LISTbalanceBST(Titles);
            break;

    }



}

int main(){
    TitleList Titles;   //collezione di titoli
    int command;

    //inizializzazione della collezione di titoli
    Titles = TITLE_LISTinit();

    do{
        fprintf(stdout,"\nMENU OPERAZIONI:\n"
                       "\t0- Fine esecuzione\n"
                       "\t1- Leggi File\n"
                       "\t2- Ricerca di un titolo\n"
                       "\t3- Dato il titolo, ricerca di una quotazione in una data specificata\n"
                       "\t4- Dato il titolo, ricerca la quotazione Min e Max in un intervallo di date\n"
                       "\t5- Dato il titolo, ricerca la quotazione Min e Max\n"
                       "\t6- Dato il titolo, bilancaiare il BST delle quotazioni\n"
                       "Comando: ");
        fscanf(stdin,"%d",&command);
        if(command<0 || command>6)
            fprintf(stdout,"Comando errato.\n\n");
        else if(command!=0)
            RunOption(Titles, command);

    }while(command!=0);

    //deallocazione della memoria
    TITLE_LISTfree(Titles);

    return EXIT_SUCCESS;
}