//
// Created by s257911 on 14/12/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventario.h"
#include "personaggi.h"


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
void RunOption(Choise ch, pglist_wrapper_p *pgl_w_p, inv_wrapper_p *iw_p){
    FILE *fp;
    char code[7], *name;
    switch(ch){
        case loadlist:
            // se risulta caricata una lista, occore liberare lo spazio prima di caricare la successiva
            if((*pgl_w_p)->head!=NULL)
                FreePlayerList_wrapper(*pgl_w_p);
            fprintf(stdout,"Caricamento lista dei players\n");
            fp=LoadFile();
            if(fp==NULL)
                printf("File non trovato\n");
            else
                LoadPlayerWrapper(fp,pgl_w_p);
            fclose(fp);
            break;
        case loadequip:
            // se risulta caricato un inventario, occore liberare lo spazio prima di caricare quello nuovo
            if((*iw_p)->vect!=NULL)
                FreeVectItems(iw_p);    // deallocazione del vettore di elementi
            fprintf(stdout,"Caricamento inventario\n");
            fp=LoadFile();
            if(fp==NULL)
                fprintf(stdout,"FileNon trovato\n");
            else
                LoadInventory_wrapper(fp,iw_p);
            fclose(fp);
            break;
        case newpg:
            fprintf(stdout,"Cariamento di un nuovo personaggio\n");
            (*pgl_w_p)->dim++;
            LoadNewPlayer(pgl_w_p);
            fprintf(stdout,"caricamento avvenuto con successo\n");
            break;
        case delpg:
            fprintf(stdout,"Eliminazione del personaggio\n");
            if(CheckEmptyList(*pgl_w_p)){
                DeletePlayer(pgl_w_p);
                (*pgl_w_p)->dim--;
                if((*pgl_w_p)->dim==0){
                    (*pgl_w_p)->head=NULL;
                    (*pgl_w_p)->head=NULL;
                }

            }
            break;
        case newequip:
            if(CheckEmptyList(*pgl_w_p) && CheckEmptyInventory(*iw_p)){
                fprintf(stdout,"\nLISTA PG:\n");
                PrintList_wrapper(*pgl_w_p);
                fprintf(stdout,"\nLISTA OGGETTI\n");
                PrintInventory_wrapper(*iw_p);
                AddItem2Pg(pgl_w_p,iw_p);
            }
            break;
        case delequip:
            if(CheckEmptyList(*pgl_w_p)) {
                fprintf(stdout, "\nLISTA PG:\n");
                PrintList_wrapper(*pgl_w_p);
                fprintf(stdout, "\nLISTA OGGETTI\n");
                PrintInventory_wrapper(*iw_p);
                DeleteItemFromPG(pgl_w_p, iw_p);
            }
            break;
        case searchpg:
            if(CheckEmptyList(*pgl_w_p)){
                fprintf(stdout,"Inserisce il codice del personaggio: ");
                fscanf(stdin,"%s",code);
                SearchPg(*pgl_w_p, code);
            }
            break;
        case searchitem:
            if(CheckEmptyInventory(*iw_p)){
                PrintInventory_wrapper(*iw_p);
                name=(char *)malloc(size * sizeof(char));
                fprintf(stdout,"\nInserisci il nome: ");
                fscanf(stdin,"%s",name);
                SearchItemByName(*iw_p,name);
                free(name);
            }
            break;
        case stats:
            if(CheckEmptyList(*pgl_w_p))
                PrintStats(pgl_w_p);
            break;

    }
}

int main(){
    unsigned int n, fine=0;
    Choise ch;
    pglist_wrapper_p pgl_w_p=AllocPlayerList_wrapper();     // allocazione della memoria per la struct wrapper
    // della lista di player
    inv_wrapper_p iw_p=(inv_wrapper_p)malloc(sizeof(inv_w));// diciarazione del puntatore alla struct
    // wrapper dell'inventario e allocazione della memoria necessaria

    InventorySetVoid(&iw_p);        // inizializzazione dell'inventario vuoto
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
        else RunOption(ch,&pgl_w_p,&iw_p);
    }

    // SISTEMARE LE FREE
    // deallocazione della memoria occupata dall'inventario e la sua struct wrapper
    FreeVectItems(iw_p);
    // deallocazione della memoria occupata dalla lista di player e la sua struct wrapper
    FreePlayerList_wrapper(pgl_w_p);


    return EXIT_SUCCESS;
}