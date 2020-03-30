//
// Created by s257911 on 14/12/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "personaggi.h"


// FUNZIONI PER LA GESTIONE DELLA LISTA DI PLAYER
int CheckEmptyList(pglist_wrapper_p pgl_w_p){
    // controllo che la lista non sia vuota
    if(pgl_w_p->head == NULL){
        fprintf(stdout,"La lista e' vuota.\n");
        return 0;
    }
    return 1;
}
void EquipSetVoid(equip_p equi){
    equi->n=0;
    equi->vect=NULL;
}
void PrintList(link_p h){
    link_p x;
    for(x=h; x!=NULL; x=x->next_p){
        // caratteristiche del pg
        fprintf(stdout,"%s %s %s %d %d %d %d %d %d\n",x->pg.cod,x->pg.name,x->pg.class,
                x->pg.stat.hp,x->pg.stat.mp,x->pg.stat.atk,x->pg.stat.def,
                x->pg.stat.mag,x->pg.stat.spr);
        // inventario del personaggio
        for(int i=0;i<x->pg.pgequip->n;i++)
            fprintf(stdout,"\t%d- %s %s %d %d %d %d %d %d\n",i+1,x->pg.pgequip->vect[i]->name,x->pg.pgequip->vect[i]->type,
                    x->pg.pgequip->vect[i]->stat.hp,x->pg.pgequip->vect[i]->stat.mp,
                    x->pg.pgequip->vect[i]->stat.atk,x->pg.pgequip->vect[i]->stat.def,
                    x->pg.pgequip->vect[i]->stat.mag,x->pg.pgequip->vect[i]->stat.spr);
    }

}
void PrintList_wrapper(pglist_wrapper_p pgl_w_p){
    PrintList(pgl_w_p->head);
}
void SearchPg(pglist_wrapper_p pgl_w_p, char *cod){
    link_p x, temp;
    // controllo che il personaggio sia presente nella lista
    for(x=pgl_w_p->head; strcmp(x->pg.cod,cod)!=0; x=x->next_p);
    if(x==NULL){
        fprintf(stdout,"\tPG non trovato.\n");
        return;
    }
    // aggiorno temoraneamente il puntatore al prossimo nodo con NULL
    temp=x->next_p;
    x->next_p=NULL;
    PrintList(x);
    x->next_p=temp;
    return;
}
pglist_wrapper_p AllocPlayerList_wrapper(){
    pglist_wrapper_p pgl_w_p;
    // allocazione della struct wrapper per la lista di player
    pgl_w_p=(pglist_wrapper_p)malloc(sizeof(pgl_w));
    // inizializzo il numero di player a 0
    pgl_w_p->dim=0;
    pgl_w_p->head=NULL;
    pgl_w_p->tail=NULL;
    return pgl_w_p;
}
void FreePlayerListR(link_p h){
    // condizione di terminazione
    if(h==NULL)
        return;
    // discesa ricorsiva
    FreePlayerListR(h->next_p);
    free(h->pg.name);
    free(h->pg.class);
    free(h->pg.pgequip->vect);
    free(h->pg.pgequip);
    free(h);
}
void FreePlayerList_wrapper(pglist_wrapper_p pgl_w_p){
    pgl_w_p->dim=0;
    FreePlayerListR(pgl_w_p->head);
    free(pgl_w_p->head);
    free(pgl_w_p->tail);
}
void FreePlayer(pglist_wrapper_p *pgl_w_p){
    FreePlayerList_wrapper(*pgl_w_p);
    free(*pgl_w_p);
}
link_p NewPlayer(link_p next, Player temp){
    link_p x = malloc(sizeof *x);
    if(x==NULL)
        return NULL;
    x->pg=temp;
    x->next_p=next;
    return x;
}
link_p SortPlayerListIns(link_p h, Player t){
    link_p x, p;

    /* Inserimento in testa*/
    if(h==NULL || strcmp(h->pg.cod,t.cod)>0)
        return NewPlayer(h,t);

    /* cerca posizione*/
    for(x=h->next_p, p=h; x!=NULL && strcmp(x->pg.cod,t.cod)<0; p=x, x=x->next_p);
    /* inserimento nella posizione trovata*/
    p->next_p=NewPlayer(x,t);
    return h;
}
link_p LoadListPlayer(FILE *fp, int *dim){
    link_p head=NULL;
    Player temp;
    char name[size], class[size];


    while (fscanf(fp,"%s %s %s %d %d %d %d %d %d",temp.cod, name, class, &temp.stat.hp, &temp.stat.mp,
                  &temp.stat.atk, &temp.stat.def, &temp.stat.mag, &temp.stat.spr)!=EOF){
        (*dim)++;
        temp.name=strdup(name);
        temp.class=strdup(class);
        temp.pgequip=(equip_p)malloc(sizeof(equip));
        EquipSetVoid(temp.pgequip);
        head=SortPlayerListIns(head,temp);
    }

    //PrintList(head);
    fprintf(stdout,"Caricamento avvenuto con successo\n");
    return head;
}
link_p SetPlayerTail(link_p h){
    link_p x,p;
    for(p=h,x=h->next_p; x!=NULL; p=x, x=x->next_p);
    return p;
}
void LoadNewPlayer(pglist_wrapper_p *pgl_w_p){
    Player temp;
    char n[size],c[size];
    fprintf(stdout,"<PGXXXX> <name> <class> <hp> <mp> <atk> <def> <mag> <spr>\n");
    fscanf(stdin,"%s %s %s %d %d %d %d %d %d",temp.cod,n,c,&temp.stat.hp,&temp.stat.mp,&temp.stat.atk,
           &temp.stat.def,&temp.stat.mag,&temp.stat.spr);
    temp.name=strdup(n);
    temp.class=strdup(c);
    (*pgl_w_p)->head=SortPlayerListIns((*pgl_w_p)->head,temp);
    (*pgl_w_p)->tail=SetPlayerTail((*pgl_w_p)->head);
}
void LoadPlayerWrapper(FILE *fp,pglist_wrapper_p *pgl_w_p){
    (*pgl_w_p)->head=LoadListPlayer(fp,&((*pgl_w_p)->dim));
    (*pgl_w_p)->tail=SetPlayerTail((*pgl_w_p)->head);
}
link_p PlayerExtraction(link_p head, char *key){
    link_p x,p;
    for(p=head,x=head->next_p; x!=NULL && strcmp(x->pg.cod,key)<=0; p=x,x=x->next_p){
        if(strcmp(x->pg.cod,key)==0){
            p->next_p=x->next_p;
            //free(x->next_p);
            return x;
        }
    }
    return NULL;
}
void DeletePlayer(pglist_wrapper_p *pgl_w_p){
    link_p t;
    char key_cod[7];
    fprintf(stdout,"Inserisci il codice univoco del personaggio: ");
    fscanf(stdin,"%s",key_cod);
    t=PlayerExtraction((*pgl_w_p)->head,key_cod);
    if(t==NULL)
        fprintf(stdout,"Player non trovato\n");
    else{
        fprintf(stdout,"Giocatore eliminato:\n"
                       "%s %s %s %d %d %d %d %d %d",t->pg.cod,t->pg.name,t->pg.class,
                t->pg.stat.hp,t->pg.stat.mp,t->pg.stat.atk,
                t->pg.stat.def,t->pg.stat.mag,t->pg.stat.spr);
        free(t->pg.name);
        free(t->pg.class);
        free(t);
    }
    (*pgl_w_p)->tail=SetPlayerTail((*pgl_w_p)->head);
}
void AddItem2Pg(pglist_wrapper_p *pgl_w_p, inv_wrapper_p *iw_p){
    int index;
    char cod[7];
    link_p x;
    fprintf(stdout,"Inserire l'indice dell'oggetto: ");
    fscanf(stdin,"%d",&index);index--;
    // controllo che l'indice sia valido
    if(index<0 || index>(*iw_p)->dim){
        fprintf(stdout,"Indice oggetto errato\n");
        return;
    }
    fprintf(stdout,"Inserire il codice del personaggio: ");
    fscanf(stdin,"%s",cod);
    // trovo il pg richiesto
    for(x=(*pgl_w_p)->head; strcmp(x->pg.cod,cod)!=0; x=x->next_p);
    if(x==NULL){    // controllo che sia presente nella lista
        fprintf(stdout,"Personaggio non trovato\n");
        return;
    }
    // se è stato trovato e il suo inventario è pieno, segnalo l'errore
    if(x->pg.pgequip->n==(*iw_p)->maxequip){
        fprintf(stdout,"L'inventario del personaggio è pieno\n");
        return;
    }
    // se è stato trovato e il suo inventario non è pieno, aggiungo l'item richiesto
    ((*x).pg.pgequip->n)++;
    x->pg.pgequip->vect=(Item **)realloc((*x).pg.pgequip->vect,((*x).pg.pgequip->n) * sizeof(Item*));
    x->pg.pgequip->vect[(*x).pg.pgequip->n-1]=&(*iw_p)->vect[index];
}
void DeleteItemFromPG(pglist_wrapper_p *pgl_w_p, inv_wrapper_p *iw_p){
    char *cod=(char *)malloc(size * sizeof(char));
    link_p x;
    int index,trovato=0,i;

    // richiesta del codice del personaggio
    fprintf(stdout,"Codice Pg: ");
    fscanf(stdin,"%s",cod);
    // controllo che il personaggio sia presente nella lista
    for(x=(*pgl_w_p)->head; strcmp(x->pg.cod,cod)!=0; x=x->next_p);
    if(x==NULL){
        fprintf(stdout,"Personaggio non trovato\n");
        return;
    }
    // controllo che l'inventario del pg non sia vuoto
    if(x->pg.pgequip->n==0){
        fprintf(stdout,"L'invetario di %s e' vuoto\n",cod);
        return;
    }
    // se è stato trovato e l'inventario non è vuoto, chiedo il codice dell'oggetto da rimuovere
    fprintf(stdout,"Inserire l'indice dell'oggetto: ");
    fscanf(stdin,"%d",&index);index--;
    // controllo che l'indice sia valido
    if(index<0 || index>(*iw_p)->dim){
        fprintf(stdout,"Indice oggetto errato\n");
        return;
    }
    // controllo che il pg abbia effettivamente l'oggetto richiesto
    for (i = 0; i < x->pg.pgequip->n && trovato==0; i++) {
        if(x->pg.pgequip->vect[i]==&(*iw_p)->vect[index])
            trovato=1;
    }
    // se viene aggiornato il flag, l'oggetto è stato trovato, altrimenti il pg non possiede l'oggetto
    if(trovato==0){
        fprintf(stdout,"L'invetario di %s non contiene l'oggetto richiesto\n",cod);
        return;
    }
    // se viene trovato, allora sicuramente si trova all'indice i
    while (i<x->pg.pgequip->n){
        x->pg.pgequip->vect[i]=x->pg.pgequip->vect[i+1];
        i++;
    }
    (x->pg.pgequip->n)--;
    x->pg.pgequip->vect=(Item **)realloc(x->pg.pgequip->vect,x->pg.pgequip->n * sizeof(Item *));

    free(cod);
}
void PrintStats(pglist_wrapper_p *pgl_w_p){
    char cod[7];
    link_p x;
    stat_t temp;
    int i;


    // richiesta del codice del personaggio
    fprintf(stdout,"Codice Pg: ");
    fscanf(stdin,"%s",cod);
    // controllo che il personaggio sia presente nella lista
    for(x=(*pgl_w_p)->head; strcmp(x->pg.cod,cod)!=0; x=x->next_p);
    if(x==NULL){
        fprintf(stdout,"Personaggio non trovato\n");
        return;
    }
    // se viene trovato e il suo inventario è vuoto, stampo solo le caratteristiche di base
    if(x->pg.pgequip->n==0){
        fprintf(stdout,"L'invetario di %s e' vuoto\n",cod);
        fprintf(stdout,"\thp:%d, mp:%d, atk:%d, def:%d, mag:%d, spr:%d\n",x->pg.stat.hp,x->pg.stat.mp,x->pg.stat.atk,
                x->pg.stat.def,x->pg.stat.mag,x->pg.stat.spr);
        return;
    }
    // se viene trovato e il suo inventario non è vuoto, calcolo le stats effettive
    fprintf(stdout,"Stats base di %s %s %s:\n\thp:%d, mp:%d, atk:%d, def:%d, mag:%d, spr:%d\n",
            x->pg.cod,x->pg.name,x->pg.class,x->pg.stat.hp,x->pg.stat.mp,x->pg.stat.atk,
            x->pg.stat.def,x->pg.stat.mag,x->pg.stat.spr);
    fprintf(stdout,"Inventario:\n");
    PrintInventory_wrapper(&inv_w);
    fprintf(stdout,"Stats effettive:\n");
    temp.hp=x->pg.stat.hp;
    temp.mp=x->pg.stat.mp;
    temp.atk=x->pg.stat.atk;
    temp.def=x->pg.stat.def;
    temp.mag=x->pg.stat.mag;
    temp.spr=x->pg.stat.spr;

    for (i = 0; i < x->pg.pgequip->n; i++){
        ((temp.hp + x->pg.pgequip->vect[i]->stat.hp)<0)? temp.hp=1 : (temp.hp += x->pg.pgequip->vect[i]->stat.hp);
        ((temp.mp + x->pg.pgequip->vect[i]->stat.mp)<0)? temp.hp=1 : (temp.mp += x->pg.pgequip->vect[i]->stat.mp);
        ((temp.atk + x->pg.pgequip->vect[i]->stat.atk)<0)? temp.hp=1 : (temp.atk += x->pg.pgequip->vect[i]->stat.atk);
        ((temp.def + x->pg.pgequip->vect[i]->stat.def)<0)? temp.hp=1 : (temp.def += x->pg.pgequip->vect[i]->stat.def);
        ((temp.mag + x->pg.pgequip->vect[i]->stat.mag)<0)? temp.hp=1 : (temp.mag += x->pg.pgequip->vect[i]->stat.mag);
        ((temp.spr + x->pg.pgequip->vect[i]->stat.spr)<0)? temp.hp=1 : (temp.spr += x->pg.pgequip->vect[i]->stat.spr);
    }
    fprintf(stdout,"\thp:%d, mp:%d, atk:%d, def:%d, mag:%d, spr:%d\n",temp.hp,temp.mp,temp.atk,
            temp.def,temp.mag,temp.spr);

}


