//
// Created by rosar on 17/11/2019.
//

/*  CREDITS
 *  Rosario Francesco Cavelli, matricola: s257911
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Costanti globali
#define max_char 50
#define max_words 30
#define max_riga 200
//Dichiaro un nuovo tipo di dato nel quale salvare una parola e la rispettiva codifica
typedef struct{
    char parola[max_char];
    char codifica[max_char];
} Parola;
//Prototipi delle funzioni
int LoadDictionary(Parola dict[max_words],FILE *fp);       //F.1: Caricamento del dizionario in un vettore di strutture
void Codifica(int dim, Parola dict[max_words],FILE *sorg, FILE* cod);   //F.2: Codifica del testo
void StringSelection(char parola[max_char],char temp[max_char],int start, int lung);


int main() {
    FILE *sorg, *diz, *cod;                                        //Variabili principali
    Parola dizionario[max_words];
    int dim;

    diz=fopen("dizionario.txt","r");            //Apro il file nel quale è contenuto il dizionario
    if(diz==NULL){                                           // Controllo che l'apertura avvenga correttamente
        printf("Dizionario non presente!\n");
        return (EXIT_FAILURE);
    }

    dim=LoadDictionary(dizionario,diz);                         //Richiamo la funzione di caricamento del dizionario
    printf("Dizionario caricato correttamente!\n");
    fclose(diz);

    sorg=fopen("sorgente.txt","r");         //Apro il file nel quale è contenuto il testo da caricare
    if(sorg==NULL){                                          // Controllo che l'apertura avvenga correttamente
        printf("Sorgente non presente!\n");
        return (EXIT_FAILURE);
    }

    cod=fopen("ricodificato.txt","w");      //Apro il file nel quale caricare il testo codificato
    if(cod==NULL){                                           //Controllo che l'apertura avvenga correttamente
        printf("Errore file 'ricodificato.txt'!\n");
        return (EXIT_FAILURE);
    }

    Codifica(dim,dizionario,sorg,cod);                          //Richiamo la funzione di codifica
    printf("Testo codificato correttamente!\n");
    fclose(sorg); fclose(cod);

    return 0;
}

int LoadDictionary(Parola dict[max_words],FILE *fp){
    int dim;
    char riga[max_riga];
    //Leggo la dimensione del file 'dizionario'
    fscanf(fp,"%d",&dim);
    for(int i=0; i<dim; i++) {
        //Acquisisco le parole salvate nel dizionario
        fscanf(fp,"%s %s",dict[i].codifica,dict[i].parola);
    }
    return dim;
}
void StringSelection(char parola[max_char],char temp[max_char],int start, int lung){
    int k;
    for (k=0; k<lung && (start+k<strlen(parola)); k++) {/*for caricare in 'temp' la sottostringa desiderata*/
        temp[k] = parola[k + start];
    }
    temp[k] = '\0';

}
void Codifica(int dim,Parola dict[max_words],FILE *sorg, FILE* cod){
    char ch,parola[max_char],temp[max_char];        //DICHIARAZIONE VARIABILI DI FUNZIONE
    int i,flag,start;
    i=0;
    while(fscanf(sorg,"%c",&ch)!=EOF){      //Leggo il file carattere per carattere
        if(ch!=' ' && ch!='\n'){                    //Se il carattere è una lettera lo salvo all'interno della parola
            parola[i]=ch;
            i++;                                    //Incremento l'indice della parola
        }
        else{
            parola[i]='\0'; //Appena trovo uno spazio termino la stringa con '\0'
            i=0;
            start=0;        /*Start è il contatore che identifica la cella di
                             * partenza dalla quale iniziare a controllare le sottostringhe di 'parola'.
                             * Condizioni: 0<start<strlen(parola)
                             */
            while(start<strlen(parola)) {
                flag=0;                     //Segnala se è avvenuta una codifica

                /*Per controllare tutte le possibili sostituzioni occorre confrontare tutte le
                 parole del dizionario con le possibili sottostringhe il cui primo carattere sarà 'parola[start]'.
                 La lunghezza di ciascuna sottostringa dipende dalla parola del dizionario che si sta
                 confrontando. Se la dict[j].parola è lunga 'l', confronto questa parola con la sottostringa di
                 parola i cui caratteri vanno da 'start' a 'start+l'. In questo modo per ogni carattere posso
                 controllare tutte le sottostringhe compatibili dimensionalmente con le parole del dizionario.

                 Una volta avvenuta la codifica (ovvero la stampa del primo codice corrispondente trovato nel
                 dizionario), il carattere da cui partire con la prossima ricerca è uguale a
                 'start+=strlen(temp)', dove 'temp' contiene la sottostringa di 'parola' che è stata codificata.

                 Se nessuna stringa codificabile inizia con il carattere contenuto in 'parola[start]',
                 allora stampo il carattere non codificato.
                 */
                for (int j=0; j<dim; j++) { //Ciclo for per confronatre tutte le parole del dizionario
                    //j è l'indice delle parole
                    StringSelection(parola,temp,start,strlen(dict[j].parola));
                    if (strlen(temp) > 0 && strcmp(temp, dict[j].parola) == 0) {    //Confronto la sottostringa con la corrente parola del dizionario
                        fprintf(cod,"%s", dict[j].codifica);                //dimensionalmente compatibile. Se coincidono stampo il codice...
                        flag = 1;                                                   //aggiorno il segnalatore dell'avvenuta codifica...
                        start += strlen(temp);                                      //aggiorno l'indice del carattere di partenza
                    }
                }
                if (flag == 0) {                                    //Se flag=0, allora nessuna sottostringa codificabile
                    fprintf(cod,"%c", parola[start]);      //inizia con 'parola[start]', quindi stampo il carattere..
                    start++;                                       //e aggiorno l'indice del carattere di partenza
                }
            }
            if(ch==' ')                     //Al termine della codifica della parola, se il carattere che
                fprintf(cod," ");  //ne ha dato il via è uno spazio, stampo uno spazio nel file ricodificato,
            else if(ch=='\n')              //se invece è uno '\n', stampo lo stesso carattere nel nuovo file.
                fprintf(cod,"\n");
        }
    }
}
