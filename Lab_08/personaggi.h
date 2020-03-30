//
// Created by s257911 on 14/12/2019.
//

#ifndef _PERSONAGGI_H
#define _PERSONAGGI_H

#include "inventario.h"



typedef struct pglist_wrapper* pglist_wrapper_p;  // alias per il puntatore alla struct wrapper della lista di pg
typedef struct node_p* link_p;       // alias per il puntatore al prossimo nodo
struct pglist_wrapper{
    int dim;
    link_p head;
    link_p tail;
}pgl_w;     // struct wrapper per la lista di players
typedef struct{
    char cod[7], *name, *class;
    equip_p pgequip;
    stat_t stat;
}Player;           // caratteristiche di ogni player
struct node_p{
    Player pg;
    link_p next_p;
};                  // nodo della lista di player

// FUNZIONI PER LA GESTIONE DELLA LISTA DI PLAYER
int CheckEmptyList(pglist_wrapper_p pgl_w_p);
void PrintList_wrapper(pglist_wrapper_p pgl_w_p);       // stampa della lista
void SearchPg(pglist_wrapper_p pgl_w_p, char *cod);
pglist_wrapper_p AllocPlayerList_wrapper();     //allocazione della lista
void FreePlayerList_wrapper(pglist_wrapper_p pgl_w_p);  //free della lista
void LoadNewPlayer(pglist_wrapper_p *pgl_w_p);                      //inserimento di un personaggio
void LoadPlayerWrapper(FILE *fp,pglist_wrapper_p *pgl_w_p);         //acquisizione da file delle informazioni dei personaggi
void DeletePlayer(pglist_wrapper_p *pgl_w_p);                       //eliminazione di un personaggio
void AddItem2Pg(pglist_wrapper_p *pgl_w_p, inv_wrapper_p *iw_p);    //aggiunta all'equip.
void DeleteItemFromPG(pglist_wrapper_p *pgl_w_p, inv_wrapper_p *iw_p);  // rimozione di un oggetto
void PrintStats(pglist_wrapper_p *pgl_w_p);    //stampa delle stats di un personaggio

#endif  //_PERSONAGGI_H