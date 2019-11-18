//
// Created by rosar on 17/11/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define size 30
#define max 1000

typedef enum Choise{        // enumerazione rappresentativa delle scelte possibili
    print_f, print_v, ord_date, ord_cod, ord_part, ord_arr, search, end, err
}choise;
typedef enum Ord{           // enumerazione per tenere traccia del tipo di ordinamento corrente
    date, codice, partenza, arrivo, null
}ord;
typedef struct{         // struttura identificativa di ogni corsa
    char cod[size];
    char part[size];
    char dest[size];
    int data,rit;
    char par[size], arr[size];
}Trt;

void SetString(char com[],unsigned const int n){    // funzione per formattare le stringhe a caratteri minuscoli
    if(com[n-1]=='\n')      // le stringhe acquisite con fgets hanno come ultimo carattere \n
        com[n-1]='\0';      // occorre modificarlo con il terminatore di stringa
    for (int i = 0; i < n; i++) {
        if(isalpha(com[i])>0) {
            com[i]=tolower(com[i]);
        }
    }
}
int LoadFile(Trt tratte[max],int *dim){     // funzione di caricamento dei dati dal file
    FILE *fp;
    int rig, gg,mm,aa;
    char data[11];
    fp=fopen("file_lab4\\corse.txt","r");
    if(fp==NULL)
        return err;
    fscanf(fp,"%d",&rig);
    *dim=rig;       // la dimensione del vettore di struct deve essere nota a tutto il programma
    for (int i = 0; i < rig; i++){
        fscanf(fp,"%s %s %s %s %s %s %d",tratte[i].cod,tratte[i].part,tratte[i].dest,data,tratte[i].par,tratte[i].arr,&tratte[i].rit);
        sscanf(data,"%d/%d/%d",&gg,&mm,&aa);    // divido la data in 3 variabili
        tratte[i].data=(aa*10000)+(mm*100)+gg;          // converto la data in un intero
        SetString(tratte[i].cod,strlen(tratte[i].cod));
        SetString(tratte[i].part,strlen(tratte[i].part));
        SetString(tratte[i].dest,strlen(tratte[i].dest));
    }
    fclose(fp);
    return 0;
}
choise Command(char comando[]){           // menu
    if(strcmp(comando,"stampa")==0){    // scelta tra la stampa a video o su file
        printf("\n- Stampa su file \t[cmd:file]\n- Stampa a video \t[cmd:video]\n");
        fgets(comando,size,stdin);      // riciclo il comando stampa per la scelta
        SetString(comando, strlen(comando));
        if(strcmp(comando,"file")==0)   // scelta la stampa su file
            return print_f;             // ritorno il codice corrispondente
        else if(strcmp(comando,"video")==0)     // scelta la stampa a video
            return print_v;             // ritorno il codice corrispondente
        else
            return err;                 // altrimenti ritorno il codice di errore
    }
    else if(strcmp(comando,"data")==0)  // ordinamento per date
        return ord_date;
    else if(strcmp(comando,"codice")==0)    // ordinamento per codici
        return ord_cod;
    else if(strcmp(comando,"partenza")==0)      // ordinamento per stazioni di partenza
        return ord_part;
    else if(strcmp(comando,"arrivo")==0)        // ordinamento per stazioni di arrivo
        return ord_arr;
    else if(strcmp(comando,"cerca")==0)         // comando di ricerca (totale o parziale)
        return search;
    else if(strcmp(comando,"fine")==0)          // fine esecuzione del programma
        return end;
    else
        return err;                             // comando errato, codice di errore
}
void Print(Trt *t[],int l,int r, choise type){      //Funzione per la stampa

    /*Si passano alla funzione due estremi l ed r del vettore di punatori;
    * in questo modo è possibile utilizzare la funzione sia per la
    * stampa completa dell'elenco ordinato (l=0, r=dim-1), sia la
    * stampa delle occorrennze trovate dopo la ricerca dicotomica, le
    * quali sono individuate, appunto, dai du estremi l ed r.
    * OSS: non è possibile applicare quanto detto per la ricerca
    * dicotomica a quella lineare poichè in quel caso il vettore non
    * risulta ordinato e di conseguenza le occorrenze potrebbero non
    * essere contigue.*/
    if(type==print_f){      // stampa su file del struttura puntata dal vettore di puntatori
        FILE *fpo;
        char nome[size];
        int gg,mm,aa;
        printf("Nome del file: ");
        fgets(nome,size,stdin);         // nome del file sul qual stampare
        SetString(nome,strlen(nome));
        strcat(nome,".txt");    // aggiunta dell'estensione
        fpo=fopen(nome,"w");
        for (int i = l; i <=r; i++) {
            aa = t[i]->data / 10000;                    /*occorre ritrasformare la data nelle 3 componenti: gg,mm,aa*/
            mm = (t[i]->data - aa * 10000) / 100;
            gg = t[i]->data - (aa * 10000 + mm * 100);
            fprintf(fpo, "%s %s %s %d/%d/%d %s %s %d\n", t[i]->cod, t[i]->part, t[i]->dest, gg, mm, aa,
                    t[i]->par, t[i]->arr, t[i]->rit);
        }
        fclose(fpo);
    } else if(type==print_v){       // stampa a video dell'elenco di tratte
        int gg,mm,aa;
        for (int i = l; i <=r; i++) {
            aa = t[i]->data / 10000;
            mm = (t[i]->data - aa * 10000) / 100;
            gg = t[i]->data - (aa * 10000 + mm * 100);
            fprintf(stdout, "%s %s %s %d/%d/%d %s %s %d\n", t[i]->cod, t[i]->part, t[i]->dest, gg, mm, aa,
                    t[i]->par, t[i]->arr, t[i]->rit);
        }
    }
}
void Merge(Trt *a[],Trt *b[], int l,int q,int r, choise type){      // funzione di fusione dei sottovettori sx e dx
    int i,j,k;
    i=l;        // indice di scansione del sottovettore sx
    j=q+1;      // indice di scansione del sottovettore dx
    if(type==ord_date) {        // ord. crescente delle date (a prità di data si tiene conto dell'ora di partenza)
        for (k = l; k <= r; k++) {      // scansione lineare del vettore di puntatori di supporto
            if (i > q) // se il sottovettore sx è terminato
                b[k] = a[j++];    // prendo gli elementi del sottovettore dx già ordinato
            else if (j > r) // se il sottovettore dx è terminato
                b[k] = a[i++];    // prendo gli elementi del sottovettore sx già ordinato
            else if (a[i]->data<a[j]->data || (a[i]->data==a[j]->data && strcmp(a[i]->par,a[j]->par)<0))   // se l'elemento a sx è < di quello dx, lo prendo e aumento l'indice sx
                b[k]=a[i++];
            else
                b[k]=a[j++];        // altrimenti prendo l'elemento di dx e incremento l'indice dx
        }
        for(k=l;k<=r;k++)           // copia delle disposizioni dei puntatori dal vettore di supporto a quello originario
            a[k]=b[k];
    }
    if(type==ord_cod){      // ord. crescente dei codici
        for (k = l; k <= r; k++) {
            if (i > q)
                b[k] = a[j++];
            else if (j > r)
                b[k] = a[i++];
            else if (strcmp(a[i]->cod,a[j]->cod)<0 || strcmp(a[i]->cod,a[j]->cod)==0)
                b[k]=a[i++];
            else
                b[k]=a[j++];
        }
        for(k=l;k<=r;k++)
            a[k]=b[k];
    }
    if(type==ord_part){     // ord. crescente secondo le stazioni di partenza
        for (k = l; k <= r; k++) {
            if (i > q)
                b[k] = a[j++];
            else if (j > r)
                b[k] = a[i++];
            else if (strcmp(a[i]->part,a[j]->part)<0 || strcmp(a[i]->part,a[j]->part)==0)
                b[k]=a[i++];
            else
                b[k]=a[j++];
        }
        for(k=l;k<=r;k++)
            a[k]=b[k];
    }
    if(type==ord_arr){      // ord. crescente secondo le stazioni di arrivo
        for (k = l; k <= r; k++) {
            if (i > q)
                b[k] = a[j++];
            else if (j > r)
                b[k] = a[i++];
            else if (strcmp(a[i]->dest,a[j]->dest)<0 || strcmp(a[i]->dest,a[j]->dest)==0)
                b[k]=a[i++];
            else
                b[k]=a[j++];
        }
        for(k=l;k<=r;k++)
            a[k]=b[k];
    }
}
void MergeSortR(Trt *a[],Trt *b[], int l,int r, choise type){   // funzione ricorsiva di divisione del vettore
    int q;          // indice di mezzo (pivot)
    if(r<=l)        // condizione di terminazione: sottovettore unitario o con dimensione nulla
        return;
    q=(r+l)/2;      // calcolo dell'indice pivot
    MergeSortR(a,b,l,q,type);       // chiamata ricorsiva: sottovettore sx
    MergeSortR(a,b,q+1,r,type);   // chiamata ricorsiva: sottovettore dx
    Merge(a,b,l,q,r,type);          // funzione di fusione e ordinamento
}
void MergeSort(Trt *a[], int n, choise type){       // wrapper della funzione di ordinamento
    int l=0,r=n-1;      // calcolo degli estremi del vettore a partire dalla dimensione
    Trt *b[n];          // dichiarazione del vettore di puntatori usiliaro (ha dimensione esatta, n)
    MergeSortR(a,b,l,r,type);   // chiamata della funzione di discesa ricorsiva
}
int BinSearch(Trt *vet[], int dim, int *l, int *r,char key[size]){      // funzione di ricerca dicotomica
    int m=0;
    unsigned int n=strlen(key);
    int i=0,j=dim-1;        // calcolo degli estremi del vettore
    while(i<=j){
        m=(i+j)/2;          //calcolo dell'indice centrale
        if(strncmp(vet[m]->part,key,n)==0)      // se l'elemento di mezzo coincide con la chiave, ho trovato l'estremo richiesto
            break;                              // uscita non strutturata dal ciclo while non appena trovo una chiave
        else if(strncmp(vet[m]->part,key,n)<0)  // se è successivo, la ricerca continua nel sottovettore dx
            i=m+1;
        else if(strncmp(vet[m]->part,key,n)>0)  // se è precedente, la ricerca continua nel sottovettore sx
            j=m-1;
    }
    if(j<i) {       // nel caso in cui la stazione non sia presente, gli indici sx e dx di sinvertono
        printf("Stazione non trovata!\n");
        return -1;
    }else{                          /* con la ricerca binaria si trova soltanto un indice delle occorrenze
                                     * all'interno del vettore ordinato. Occorre quindi cercare gli estremi delle
                                     * occorrenze presenti. Si sfrutta a tal proposito la contiguità delle chiavi
                                     * dovuta al precedente ordinamento.*/
        // Ricerco l'estremo sx
        *l=m;                       // assegnazione all'indice della prima occorrenza
        if((*l)-1>=0) {             // se esistono elementi precedenti
            while (strncmp(vet[(*l) - 1]->part, key, n) == 0 && (*l) - 1 >= 0)
                (*l)--;             // continuo a diminuire l'indice fino a trovare il primo elemento diverso
        }
        // Ricerco l'estremo dx
        *r=m;                       // assegnazione all'indice della prima occorrenza
        if((*r)+1<dim) {            // se esistono elementi successivi
            while (strncmp(vet[(*r) + 1]->part, key, n) == 0 && (*r) + 1 < dim)
                (*r)++;             // continuo ad aumentare l'indice fino a trovare il primo elemento diverso
        }
        return 0;
    }
}
void LinearSearch(Trt *vet[], int dim,char key[size]){      // ricerca lineare (in caso di ordinamento mancante)
    int flag=0;
    int aa,mm,gg,n=strlen(key);
    for(int i=0;i<dim;i++){     // stampa delle occorrenze trovate
        if(strncmp(vet[i]->part,key,n)==0){
            flag=1;
            aa = vet[i]->data / 10000;
            mm = (vet[i]->data - aa * 10000) / 100;
            gg = vet[i]->data - (aa * 10000 + mm * 100);
            fprintf(stdout, "%s %s %s %d/%d/%d %s %s %d\n", vet[i]->cod, vet[i]->part, vet[i]->dest, gg, mm, aa,
                    vet[i]->par, vet[i]->arr, vet[i]->rit);
        }
    }
    if(flag==0)     // se il flag non si è aggioranto, la chiave non è stata mai trovata
        printf("Stazione non trovata!\n");
}
void Operazioni(Trt tratte[],int dim){
    ord ord_corrente=date;          // tiene traccia dell'ordinamento corrente
    int repeat=1,l,r,signal;
    char key[size];                 // chiave della funzione di ricerca
    char comando[size];             // comando per la scelta dal menu

    // vettori di puntatori alle struct per mantenre un oridamento multiplo
    Trt *o_date[dim];   // ord. crescente per date
    Trt *o_cod[dim];    // ord. crescente per codici
    Trt *o_part[dim];   // ord. crescente per stazioni di partenza
    Trt *o_dest[dim];   // ord. crescente per stazioni di arrivo
    Trt **last[4]={o_date,o_cod,o_part,o_dest};       /*vettore di puntatori a puntatori.
                                                       *ciascun elemento del vettore rappresenta un puntatore a
                                                       * puntatore. L'ordine di assegnazione degli indirizzi di
                                                       * memoria è fatto secondo l'enumerazione fatta per
                                                       * rappresentare l'ordinamento corrente; in questo modo è
                                                       * possibile accedere all'apportuno ordinamento tramite
                                                       * l'accesso diretto secondo un indice (enumerativo corrispondente
                                                       * convertito ad intero)*/
    // Inizializzazione dei vettori di puntatori alle singole strutture
    for (int i = 0; i < dim; i++)
        o_date[i]=&tratte[i];
    for (int i = 0; i < dim; i++)
        o_cod[i]=&tratte[i];
    for (int i = 0; i < dim; i++)
        o_part[i]=&tratte[i];
    for (int i = 0; i < dim; i++)
        o_dest[i]=&tratte[i];
    printf("Operazioni:"              /*stampa del menu*/
           "\n- Stampa i log................................[cmd:stampa]"
           "\n- Ordinamento per data........................[cmd:data]"
           "\n- Ordinamento per codice......................[cmd:codice]"
           "\n- Ordinamento per stazione di partenza........[cmd:partenza]"
           "\n- Ordinamento per stazione di arrivo..........[cmd:arrivo]"
           "\n- Ricerca.....................................[cmd:cerca]"
           "\n- Fine........................................[cmd:fine]\n");

    // Lista delle operazioni possibili
    while(repeat) {
        printf("Comando: ");
        fgets(comando,size,stdin);      // acquisizione del comando da tastiera
        SetString(comando, strlen(comando));       // formattazione comando
        switch (Command(comando)) {
            case print_f:                           // Stampa su file
                Print(last[(int)ord_corrente], 0, dim - 1, print_f);
                break;
            case print_v:                           // Stampa a video
                Print(last[(int)ord_corrente], 0, dim - 1, print_v);
                break;
            case ord_date:                          // Ordinamento per date (crescente)
                ord_corrente = date;
                MergeSort(o_date, dim, ord_date);
                break;
            case ord_cod:                           // Ordinamento per codici (crescente)
                ord_corrente = codice;
                MergeSort(o_cod, dim, ord_cod);
                break;
            case ord_part:                          // Ordinamento per partenze
                ord_corrente = partenza;
                MergeSort(o_part, dim, ord_part);
                break;
            case ord_arr:                           // Ordinamento per arrivo
                ord_corrente = arrivo;
                MergeSort(o_dest, dim, ord_arr);
                break;
            case search:                            // Ricerca (parziale o totale)
                printf("Inserisci chiave: ");
                fgets(key,size,stdin);
                SetString(key,strlen(key));
                if(ord_corrente==partenza) {        // ordinamento corrente corretto
                    signal = BinSearch(last[(int)ord_corrente], dim, &l, &r, key);      // ricerca binaria
                    if(signal==0)                   // non è stata trovata la chiave (non c'è nulla da stampare)
                        Print(last[(int)ord_corrente],l,r,print_v);
                }
                else
                    LinearSearch(last[(int)ord_corrente], dim,key);                     // ricerca lineare
                break;
            case end:                               // fine esecuzione
                printf("Esecuzione terminata.\n");
                repeat=0;
                break;
            default:                                // comando non valido
                printf("Comando non valido.\n");
                break;
        }
    }

}

int main() {
    int dim;
    choise signal;
    Trt tratte[max];    // vettore di struct

    // Gestione input da file
    signal = LoadFile(tratte, &dim);
    if (signal == err) {
        printf("File non trovato.\n");
        return (EXIT_FAILURE);
    }

    Operazioni(tratte, dim);        // chiamata al menu delle scelte

    return 0;
}
