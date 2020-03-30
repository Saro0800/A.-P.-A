//
// Created by Rosario on 24/11/2019.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define size 50

typedef enum{       // enumerazione delle scelte possibili
    input_t, input_f, search_c, estr_c, estr_d, output_v, output_f, end, err
}Choise;
typedef struct {        // nuovo dato per la data inserita
    int gg, mm, aaaa;
}DATA;
typedef struct{     // struttura di un item
    char cod[6];          // formato Axxxx con x 0-9
    char *nome;
    char *cognome;
    char *via;
    char *citta;
    int cap;
    DATA data;
}Item;
typedef struct node_t *link;    // alias del puntatore a nodo
struct node_t{  // struct nodo della lista
    Item val;   // dati effettivi
    link next;  // puntatore al successivo
};

int data2int(DATA data){        // funzione di conversione in intero
    int d;
    d=data.aaaa*10000+data.mm*100+data.gg;
    return d;
}
int KEYDategeq(DATA data1, DATA data2){     // funzione di controllo che data1 è minore di data2
    int d1=data2int(data1);         // data1 viene convertita in intero
    int d2=data2int(data2);         // data2 viene convertita in intero
    if(d1>=d2)
        return 1;
    else
        return 0;
}
int KEYDateleq(DATA data1, DATA data2){     // funzione di controllo che data1 è minore di data2
    int d1=data2int(data1);         // data1 viene convertita in intero
    int d2=data2int(data2);         // data2 viene convertita in intero
    if(d1<=d2)
        return 1;
    else
        return 0;
}
int KEYCodeq(char *cod1, char *cod2){       // funzione di controllo che due codici siano uguali
    return !strcmp(cod1,cod2);  // si restituisce 0 se i codici sono diversi, 1 se i codici sono uguali
}
int Itemeq(Item t1, Item t2){       // funzione di controllo che due item siano uguali
    int flag=0;
    if(strcmp(t1.cod,t2.cod)==0)
        flag=1;
    return flag;                    // restituisce il flag: 0 se diversi, 1 se uguali
}
int CheckPerson(Item t, link head){     // funzione di controllo se un item è già contenuto nella lista
    link x;
    int flag=0;                         // flag di segnalazione di ritrovamento dell'item
    for(x=head; x!=NULL && flag==0; x=x->next){ // scansione della lista finchè non termina o finchè non viene trovato l'item richiesto
        if(Itemeq(t,x->val))            // controllo che il val del nodo corrente sia uguale a quello passato alla funzione
            flag=1;                     // aggiornamento del flag di ritrovamento
    }
    return flag;                        // la funzione restituisce il valore del flag: 0 non trovato, 1 trovato
}
void FreeNode(link x){      // funzione di deallocazione dei campi di un nodo
    if(x==NULL)
        return;
    free(x->val.nome);
    free(x->val.cognome);
    free(x->val.via);
    free(x->val.citta);
    FreeNode(x->next);
    free(x->next);
}
void PrintList(link h,FILE *fp){        // funzione di stampa della (porzione di) lista
                                        // funzione ricorsiva
    link x=h;
    if(x==NULL)                         // condizione di terminazione: lista finita
        return;
    fprintf(fp, "%s %s %s %2d/%2d/%4d %s %s %d\n", x->val.cod, x->val.nome, x->val.cognome,
            x->val.data.gg, x->val.data.mm, x->val.data.aaaa, x->val.via, x->val.citta, x->val.cap);
    PrintList(x->next,fp);              // continuazione ricorsiva sul nodo successivo della lista
}
void PrintListFile(link h){     // funzione di stampa su file
                                // può essere considerata una wrapper in quanto costruisce un puntatore a file
                                // e poi lo passa alla funzione di stampa vera e propria
    FILE *fp;
    char filename[size];
    printf("Nome File: ");
    fscanf(stdin,"%s",filename);
    if(strstr(filename,".txt")==NULL)
        strcat(filename,".txt");
    fp=fopen(filename,"w");
    if(fp==NULL){
        printf("File non trovato!\n");
        return;
    }
    PrintList(h,fp);            // richiamo la funzione di stampa
    printf("File stampato!\n");
}
void SearchR(link head, char *cod){     // funzione di ricerca ricorsiva
    link x=head;         // puntatore al nodo corrente
    link temp;           // puntatore temporeaneo al nodo successivo

    if(x==NULL){        // condizione di terminazione: se raggiunta, non è stato trovato il codice
        printf("Codice non trovato!\n");
        return;
    }

    if(KEYCodeq(cod,x->val.cod)){   // codice cercato è uguale a quello del nodo corrente
        temp=x->next;               // si salva il puntatore al successore in uno temporaneo
        x->next=NULL;               // si trasforma il puntatore al successore in null
        PrintList(x,stdin);         // viene chiamanta la funzione di stampa
                                    // essa continua a stampre i nodi finchè x->next != NULL, per questo si
                                    // modifica temporaneamente il puntatore al successore
        x->next=temp;               // viene riassegnato il puntatore al successore
        return;                     // interruzione della ricorsione
    }
    SearchR(x->next,cod);           // continuazione ricorsiva sul nodo successore
    return;
}
void SetString(char *c){        // funzione per trasformare i caratteri in minuscolo
    for(int i=0; c[i]!='\0';i++)
        c[i]=tolower(c[i]);
}
link DelNode(link *h, DATA d1, DATA d2, int *fine){
    link *x, t;
    for(x=h; *x!=NULL; x=&((*x)->next)){
        if(KEYDategeq((*x)->val.data,d1) && KEYDateleq((*x)->val.data,d2)){     // estrazione del nodo
            t=*x;
            *x=(*x)->next;
            t->next=NULL;
            return t;
        }
    }
    *fine=1;
    return NULL;

}
void DeleteDateNode(link h, DATA d1, DATA d2){      // funzione di estrazione in base a due date
    int fine=0;
    link del;
    printf("Nodi estatti:\n");
    while (!fine){      // finche non viene segnalato dalla funzione di eliminazione che è stato completata
                        // almeno una scansione di tutta lista, quindi non ci sono più nodi con data compresa tra le due
                        // indicate
        del=DelNode(&h, d1, d2, &fine);     // funzione di estrazione di un nodo
        if(del!=NULL) {       // se viene restituito un vettore non nulla, si esegue la stampa del nodo estratto
            PrintList(del, stdout); // stampa a video
            /* deallocazione*/
            FreeNode(del);
            free(del);
        }
    }
}
link newNode(Item val, link next){      // funzione di creazione del nuovo nodo
    link x=(link)malloc(sizeof *x);     // allocazione dinamica del nodo
    if(x==NULL)                         // gestione errore relativo alla mancanza della memoria
        return NULL;
    x->val=val;                         // assegnazione dei campi dell'item
    x->next=next;                       // assegnazione del puntatore al nodo successivo
    return x;                           // restituzione del puntatore al nodo creato
}
link SortListIns(link h, Item val){
    link x;                 // puntatore al nodo corrente
    link p;                 // puntatore al predecessore
    DATA k=val.data;      // campo di temp soggetto all'ordinamento

    /*Inserimento in testa*/
    if(h==NULL || KEYDategeq(k,h->val.data))
        return newNode(val,h);

    /*Trovo la posizione dove inserire il nodo*/
    for(x=h->next,p=h;                              // ricerca del predecessore del nodo da inserire
        x!=NULL && KEYDategeq(h->val.data,k);
        p=x,x=x->next);
    p->next=newNode(val, x);
    return h;       // restituzione del puntatore alla testa, eventualmente modificata
}
Item ItemSet(char cod[6], char nome[size],char cognome[size], char via[size],char citta[size],DATA d,int cap){
    // Funzione di riallocazione dinamica dei campi dell'item
    Item temp;
    temp.nome = strdup(nome);
    temp.cognome = strdup(cognome);
    temp.via = strdup(via);
    temp.citta = strdup(citta);
    strcpy(temp.cod,cod);
    temp.data=d;
    temp.cap=cap;
    return temp;
}
Item ItemScan(FILE *fp, int *fine){     // unica funzione di lettura di un nodo
                                        // fp è uguale al puntatore al file nel caso in cui venga richiesto l'input da
                                        // file, oppure stdin se viene richiesto l'input da tastiera
    char nome[size],cognome[size],via[size],citta[size];        // stringhe di supporto per l'input
    Item temp;      // Item temporaneo per la lettura da file
    if(fscanf(fp,"%s %s %s %2d/%2d/%4d %s %s %d",temp.cod,nome,cognome,
              &temp.data.gg,&temp.data.mm,&temp.data.aaaa,via,citta,&temp.cap)!=EOF)
        temp=ItemSet(temp.cod,nome,cognome,via,citta,temp.data,temp.cap);   // richiama la funzione di riallocazione dinamica dei campi dell'item
    else *fine=1;
    return temp;    // restituisce l'item acquisito
}
link Load(link head,Choise bin){        // funzione di lettura della lista
    FILE *fp;
    int fine=0;             // intero che segnala la fine o meno del file
    char filename[size];    // nome del file dal quale fare input

    Item temp;      // Item temporaneo per la lettura da file

    switch(bin){
        case input_f:
            // lettura da file
            printf("Nome File: ");          // acquisizione del nome del file di input
            fscanf(stdin,"%s",filename);
            if(strstr(filename,".txt")==NULL)   // controllo che sia presente l'estensione del file
                strcat(filename,".txt");        // se non è presente, viene aggiunta
            fp=fopen(filename,"r");         // apertura del file in modalità lettura
            if(fp==NULL){                               // gestione errore relativo a file mancante o non trovato
                printf("File non trovato!\n");
                return NULL;
            }
            while(!fine){                           // finchè il file non termina
                temp = ItemScan(fp,&fine);          // si acquisisce un Item temporaneo da file (viene passato fp per segnalare che l'input è da file)
                if(!CheckPerson(temp,head) && !fine)    // controllo che l'elemento non sia un duplicato e che non sia finito il file
                    head = SortListIns(head, temp);     // inserimento dell'item in lista
            }
            printf("Nodi Caricati.\n");
            break;
        case input_t:
            // lettura da tastiera
            printf("<codice> <nome> <cognome> <gg/mm/aaaa> <via> <citta'> <cap>\n");
            temp = ItemScan(stdin,&fine);    // acquisizione di un item temporaneo da tastiera (viene passato stdin per segnalare l'input da tastiera)
            if(!CheckPerson(temp,head)) {      // controllo che l'elemento non sia un duplicato
                head = SortListIns(head, temp);     // inserimento dell'item in lista
                printf("Nodo inserito!\n");
            }
            break;
    }
    return head;        // viene restituito il puntatore alla testa della lista
}
link DeleteCodNode(link *h,char *cod){      // funzione di cancellazione dei nodi in base al codice
    link *x,t;
    for(x=h; *x!=NULL; x=&((*x)->next)){    // controllo gli elementi della lista
        if(KEYCodeq((*x)->val.cod,cod)){    // finchè non trovo un nodo con il codice ricercato
            t=*x;                           // salvo il puntatore al nodo trovato
            *x=(*x)->next;                  // il successore di t diventa il successore di x
            t->next=NULL;                   // il successore di quello estratto viene ritornato NULL
            return t;                       // restituisco il puntatore trovato
        }
    }
}
Choise LeggiComando(char *c){               // funzione di conversione tra stringa inserita ed enumerazione
    if(strcmp(c,"input_t")==0)              return input_t;
    else if(strcmp(c,"input_f")==0)         return input_f;
    else if(strcmp(c,"ricerca")==0)         return search_c;
    else if(strcmp(c,"estrazione_cod")==0)  return estr_c;
    else if(strcmp(c,"estrazione_date")==0) return estr_d;
    else if(strcmp(c,"stampa_v")==0)        return output_v;
    else if(strcmp(c,"stampa_f")==0)        return output_f;
    else if(strcmp(c,"fine")==0)            return end;
    else                                    return err;
}
void Menu(link head){
    int fine=0;             // intero di controllo per l'iterazione del menu
    char comd[size];        // carattere corrispondente al numero del comando
    char cod[6];            // codice per la ricerca
    link del;               // puntatore alla testa della lista con i nodi estratti
    DATA data1, data2;      // date limite perl'estrazione

    printf("Menu:\n"            // visualizzazione menu
           "\t- Input elemento da tastiera.......................................[input_t]\n"
           "\t- Input elementi da file...........................................[input_f]\n"
           "\t- Ricerca per codice...............................................[ricerca]\n"
           "\t- Estrazione per codice............................................[estrazione_cod]\n"
           "\t- Estrazioni elementi con date comprese in un intervallo...........[estrazione_date]\n"
           "\t- Stampa a video...................................................[stampa_v]\n"
           "\t- Stampa su file...................................................[stampa_f]\n"
           "\t- Fine.............................................................[fine]\n");

    while (!fine){                      // iterazione della richiesta del comando
        printf("\nComando: ");    // acquisizione del comando
        scanf("%s",comd);
        SetString(comd);                // trasformo tutti i caratteri in minuscolo
        switch(LeggiComando(comd)){
            case input_t:                           // input di un nodo da tastiera
                head=Load(head,input_t);        /* viene passato il puntatore alla testa della lista alla funzione
                                                     * di caricamento dei nodi. La funzione richiede come parametro
                                                     * l'informazione che riporta da dove avviene i'input: in questo
                                                     * caso da tastiera.                                             */
                break;
            case input_f:                           // input di un nodo da file
                head=Load(head,input_f);        /* viene passato il puntatore alla testa della lista alla funzione
                                                     * di caricamento dei nodi. La funzione richiede come parametro
                                                     * l'informazione che riporta da dove avviene i'input: in questo
                                                     * caso da file.                                                 */
                break;
            case search_c:                              // ricerca di un codice all'interno della lista
                if(head==NULL)                          // se head=NULL la lista è vuota
                    printf("Lista vuota!\n");
                else{                                       // altrimenti
                    printf("Codice da cercare: ");  // acquisisco il codice da cercare
                    fscanf(stdin,"%s",cod);
                    SearchR(head,cod);                      // funzione di ricerca
                }
                break;
            case estr_c:                            // estrazione dei nodi per codici
                if(head==NULL)                      // gestione della lista vuota
                    printf("Lista vuota!\n");
                else{
                    printf("Codice da eliminare: ");    // acquisizione del codice da eliminare
                    fscanf(stdin,"%s",cod);
                    del=DeleteCodNode(&head, cod);              // del è il puntatore al nodo estratto (unico poichè ogni codice è univoco)
                    printf("Nodo estratto:\n");
                    PrintList(del,stdout);                      // stampa del nodo estratto
                    /* DEALLOCAZIONE */
                    FreeNode(del);
                    free(del);
                }
                break;
            case estr_d:                            // estrazione dei nodi tra due date
                if(head==NULL)                      // gestione lista vuota
                    printf("Lista vuota!\n");
                else{
                    do{
                        printf("data 1: ");     // acquisizione data1
                        scanf("%2d/%2d/%4d",&data1.gg,&data1.mm,&data1.aaaa);
                        printf("data 2: ");     // acquisizione data2
                        scanf("%2d/%2d/%4d",&data2.gg,&data2.mm,&data2.aaaa);
                        if(KEYDategeq(data1,data2))     // controllo errore relativo alla data1>data2
                            printf("Error: Data 2 < Data 1\n");
                    }while(KEYDategeq(data1,data2));    // se si ricade nell'errore, viene richiesto l'inserimento delle date
                    DeleteDateNode(head,data1,data2);   // richiamo la funzione wrapper
                }
                break;
            case output_v:                          // stampa a video
                if(head==NULL)                      // gestione lista vuota
                    printf("Lista vuota!\n");
                else
                    PrintList(head,stdout);         // chiamata alla funzione di stampa del nodo
                break;
            case output_f:                          // stampa su file
                if(head==NULL)                      // gestione lista vuota
                    printf("Lista vuota!\n");
                else
                    PrintListFile(head);            // chiamata alla funzione di stampa su file (wrapper)
                break;
            case end:                               // terminazione dell'iterazione
                fine=1;
                break;
            case err:                               // caso in cui venga inserito un comando errato
                printf("Comando errato!\n");
                break;
        }
    }

}

int main(){
    link head = NULL;           // puntatore alla testa della lista - creazione lista vuota

    Menu(head);                 // menu delle scelte
    FreeNode(head);
    free(head);
    return EXIT_SUCCESS;
}











