//
// Created by rosar on 17/11/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define size 100

typedef enum Type{punto, quadre, apice, minuscolo, maiuscolo, uguale}type;

type ControllExp(const char *c,int *p){
    switch (c[*p]){
        case '.':
            return punto;
        case '[':
            (*p)++;
            return (c[*p]=='^')? apice:quadre;
        case '\\':
            (*p)++;
            return (c[*p]=='A')?maiuscolo:minuscolo;
        default:
            return uguale;
    }

}

int lungcod(char *regexp){      // Determina la lunghezza della parola cercare in base alla codifica
    int cont;
    for(cont=0;regexp[cont]!='\0';cont++){
        switch(ControllExp(regexp,&cont)){
            case punto:
                cont++;
                break;
            case apice:
                for(int i=cont;regexp[i]!=']';i++,cont++);
                cont++;
                break;
            case quadre:
                for(int i=cont;regexp[i]!=']';i++,cont++);
                cont++;
                break;
            case maiuscolo:
                cont++;
                break;
            case minuscolo:
                cont++;
                break;
            case uguale:
                cont++;
                break;
        }
    }
    return cont;
}

char *cercaRegExp(char *src, char *regexp){     /* src punta al primo carattere della stringa di input e serve anche
                                                *ad indicare le parole della stringa da controllare.
                                                * regexp punta al primo carattere dell'espressione e serve anche per
                                                * il reset del puntatore di controllo espressione se la parola in esame
                                                * non soddisfa i requisiti*/
    char *sr=src;
    char *find=NULL;    // Puntatore da restituire
    char *start,word[size];
    int exit=0,failure,s=0,r=0,i=0;    /* exit per il quantificatore esistenziale 'esiste almeno uno'
                            * failure si aggiorna se una parola non soddisfa l'espressione
                            */
    int lunghezza=lungcod(regexp);

    start=&(sr[s]);
    sscanf(start, "%s", word);
    while(sr[s]!='\0' && regexp[r]!='\0') {      // Controllo i caratteri fino ad incontrare l'esurimento di una delle stringhe
        failure = 0;
        if (strlen(word) != lunghezza)      // Se la lunghezza della parola sorgente è diversa da quella ricercata non effettuo i controlli
            failure = 1;
        if(!failure){
            switch (ControllExp(regexp, &r)) {
                case punto:
                    s++;r++;
                    break;

                case quadre:
                    /*Verifico che il carattere corrente della sorgente sia uguale ad almeno uno
                     * dei caratterei specificati: in caso positivo esco anticipatamente dal while di controllo*/
                    r++;
                    while (regexp[r] != ']' && exit == 0) {
                        if (sr[s] == regexp[r])
                            exit = 1;
                        else r++;
                    }
                    if (exit == 1) {    // Se ho trovato almeno un carattere uguale
                        // Sposto re fino a ']'
                        while (regexp[r] == ']')
                            r++;
                    } else // Se non ho trovato almeno un carattere uguale, la parola non soddisfa i requisiti
                        failure = 1;
                    break;

                case apice:
                    /*Verifico che il carattere della sorgente sia diverso da tutti i caratteri indicati;
                     se è uguale ad almeno uno dei caratteri specificati, la parola non soddisfa i requisiti*/
                    r++;
                    while (regexp[r] != ']' && failure == 0) {
                        if (sr[s] == regexp[r])
                            failure = 1;
                        else r++;
                    }
                    if (failure == 0)     // Se il carattere è doverso da tutti quelli specificati
                        s++;
                    r++;
                    break;

                case minuscolo:
                    r++;
                    if (islower(sr[s]) == 0) {    // Verifico che sia minuscolo
                        s++;r++;
                    } else      // Se non è minuscolo non soddisfa i requisiti
                        failure = 1;
                    break;

                case maiuscolo:
                    r++;
                    if (isupper(sr[s]) == 0) {    // Verifico che sia maiuscolo
                        s++;r++;
                    } else      // Se non è maiuscolo non soddisfa i requisiti
                        failure = 1;
                    break;

                case uguale:
                    if (sr[s] == regexp[r]) {    // Verifico che siano uguali
                        s++;r++;
                    } else      // Se non sono uguali non soddisfa i requisiti
                        failure = 1;
                    break;
            }
        }

        if (failure == 1) {      // La parola non soddisfa i requisiti
            /*Se si è aggiornato il flag vuol dire che almeno una delle condizioni
                            *dell'espressione è stata violata; occorre quindi cambiare parola,
                            *e ciò significa che bisogna aggiornare il puntatore della sorgente e
                            *resettare quello dell'espressione*/
            // Cerco la prossima parola puntata da *src
            for (i = s; src[i] != ' ' && src[i] != '\0'; i++);      // Avanzo fino ad incontrare uno spazio o il terminatore
            for (; src[i] ==' '; i++);                  // Avanzo fino a trovare un carattere o il terminaotre stringa
            s = i;
            //  Reset del puntatore dell'espressione
            r = 0;
            start=&(sr[s]);
            sscanf(start, "%s", word);
        }
    }

    if(regexp[r]=='\0')
        find=&(src[i]);
    return find;
}

int main() {
    char source[size]="rotolo moto",expression[size]=".oto";
    char *find;
    int i=0;

    // Stringa sorgente
    printf("Stinga di input: ");
    fgets(source,size,stdin);
    source[strlen(source)-1]='\0';

    // Stringa regexp
    printf("Stinga di espressione: ");
    fgets(expression,size,stdin);
    expression[strlen(expression)-1]='\0';

    // Chiamata alla funzione di ricerca
    find=cercaRegExp(source,expression);

    if(find==NULL)
        printf("Nessuna parola trovata!\n");
    else{
        while(find[i]!='\0')
            printf("%c",find[i++]);
    }
    return EXIT_SUCCESS;
}