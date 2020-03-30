//
// Created by s257911 on 18/12/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgList.h"
#include "pg.h"
#include "invArray.h"

#define lenght 51
#define maxEquip 8

typedef enum{
    loadlist=1, loadequip, newpg, delpg, searchpg, newequip, delequip, searchitem, stats, end, err
}Choise;

FILE *LoadFile(){
    FILE *fp;
    char name[100];
    fprintf(stdout,"Nome del file: ");
    fscanf(stdin,"%s",name);
    if(strstr(name,".txt")==NULL)
        strcat(name,".txt");
    fp=fopen(name,"r");
    return fp;
}
Choise SelectOptions(unsigned int n){
    switch (n){
        case 1:
            return loadlist;
        case 2:
            return loadequip;
        case 3:
            return newpg;
        case 4:
            return delpg;
        case 5:
            return searchpg;
        case 6:
            return newequip;
        case 7:
            return delequip;
        case 8:
            return searchitem;
        case 9:
            return stats;
        case 10:
            return end;
        default:
            return err;
    }
}
void RunOption(Choise ch, pgList_p *pgList, tabInv_p *invArray){
    FILE *fp;
    char code[7], *name;
    pg_t *temp=(pg_t *)malloc(sizeof(pg_t));
    switch(ch){
        case loadlist:
            // se risulta caricata una lista, occore liberare lo spazio prima di caricare la successiva
            if(!pgList_Empty(*pgList))
                pgList_free(*pgList);
            fprintf(stdout,"Caricamento lista dei players\n");
            fp=LoadFile();
            if(fp==NULL)
                printf("File non trovato\n");
            else{
                *pgList = pgList_init();
                pgList_read(fp,*pgList);
            }
            printf("Caricamento avvenuto con successo.\n");
            fclose(fp);
            break;
        case loadequip:
            // se risulta caricato un inventario, occore liberare lo spazio prima di caricare quello nuovo
            if(!invArray_Empty(*invArray))
                tabInvFree(*invArray);   // deallocazione del vettore di elementi
            fprintf(stdout,"Caricamento inventario\n");
            fp=LoadFile();
            if(fp==NULL)
                fprintf(stdout,"FileNon trovato\n");
            else {
                *invArray = tabInvInit();
                invArray_read(fp, *invArray);
            }
            printf("Caricamento avvenuto con successo.\n");
            fclose(fp);
            break;
        case newpg:
            fprintf(stdout,"Caricamento di un nuovo personaggio\n");
            fprintf(stdout,"<PGXXXX> <name> <class> <hp> <mp> <atk> <def> <mag> <spr>\n");
            pg_read(stdin,temp);
            pgList_insert(*pgList,*temp);
            fprintf(stdout,"caricamento avvenuto con successo\n");
            break;
        case delpg:
            fprintf(stdout,"Eliminazione del personaggio\n");
            if(!pgList_Empty(*pgList)){
                fprintf(stdout,"Inserisce il codice del personaggio: ");
                fscanf(stdin,"%s",code);
                pgList_remove(*pgList, code,*invArray);
                if(pgList_Empty(*pgList)){
                    pgList_free(*pgList);
                    *pgList=pgList_init();
                }
            }
            break;
        case newequip:
            if(!pgList_Empty(*pgList) && !invArray_Empty(*invArray)){
                fprintf(stdout,"\nLISTA PG:\n");
                pgList_print(stdout,*pgList,*invArray);
                fprintf(stdout,"\nLISTA OGGETTI\n");
                invArray_print(stdout,*invArray);
                printf("\n");
                fprintf(stdout,"Inserisce il codice del personaggio: ");
                fscanf(stdin,"%s",code);
                if(pgList_searchByCode(*pgList,code)!=NULL)
                    pg_updateEquip(pgList_searchByCode(*pgList,code),*invArray,1);
            }
            break;
        case delequip:
            if(!pgList_Empty(*pgList)) {
                fprintf(stdout, "\nLISTA PG:\n");
                pgList_print(stdout,*pgList,*invArray);
                fprintf(stdout, "\nLISTA OGGETTI\n");
                invArray_print(stdout,*invArray);
                fprintf(stdout,"Inserisce il codice del personaggio: ");
                fscanf(stdin,"%s",code);
                pg_updateEquip(pgList_searchByCode(*pgList,code),*invArray,2);
            }
            break;
        case searchpg:
            if(!pgList_Empty(*pgList)){
                fprintf(stdout,"Inserisce il codice del personaggio: ");
                fscanf(stdin,"%s",code);
                pgList_searchByCode(*pgList,code);
            }
            break;
        case searchitem:
            if(!invArray_Empty(*invArray)){
                invArray_print(stdin,*invArray);
                name=(char *)malloc(lenght * sizeof(char));
                fprintf(stdout,"\nInserisci il nome: ");
                fscanf(stdin,"%s",name);
                if(invArray_searchByName(*invArray,name)<maxEquip)
                    invArray_printByIndex(stdout,*invArray,invArray_searchByName(*invArray,name));
                else printf("Oggetto non trovato.\n");
                free(name);
            }
            break;
        case stats:
            if(!pgList_Empty(*pgList)){
                fprintf(stdout,"Codice personaggio: ");
                fscanf(stdin,"%s",code);
                temp = pgList_searchByCode(*pgList,code);
                pg_printstat(temp,*invArray,temp->equipPg);
            }

            break;

    }
}

int main(){
    unsigned int n, fine=0;
    Choise ch;
    // allocazione della memoria per la struct wrapper della lista di player
    pgList_p pgList = pgList_init();

    // diciarazione del puntatore alla struct wrapper dell'inventario
    tabInv_p invArray = tabInvInit();

    // menù delle opzioni
    fprintf(stdout,"\nOPTIONS:\n"
                   "1- Caricare l'elenco di personaggi\n"
                   "2- Caricare l'elenco di oggetti\n"
                   "3- Aggiungere un nuovo personaggio\n"
                   "4- Eliminare un personaggio\n"
                   "5- Ricerca di un personaggio in base al codice\n"
                   "6- Aggiungere un oggetto dall'equipaggiamento di un personaggio\n"
                   "7- Rimuovere un oggetto dall'equipaggiamento di un personaggio\n"
                   "8- Ricerca di un oggetto per nome\n"
                   "9- Calcolare le statistiche di un personaggio\n"
                   "10- Fine\n");
    while(fine==0){
        fprintf(stdout,"\nComando: ");
        fscanf(stdin,"%d",&n);
        ch=SelectOptions(n);
        if(ch==err)
            fprintf(stdout,"Comando errato\n");
        else if(ch==end)
            fine=1;
        else RunOption(ch,&pgList,&invArray);
    }

    // SISTEMARE LE FREE
    // deallocazione della memoria occupata dall'inventario e la sua struct wrapper
    tabInvFree(invArray);
    // deallocazione della memoria occupata dalla lista di player e la sua struct wrapper
    pgList_free(pgList);

    return EXIT_SUCCESS;
}