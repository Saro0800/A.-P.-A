//
// Created by rosar on 17/11/2019.
//

/*  CREDITS:
 *  Rosario Francesco Cavelli, matricola: s257911
 */
/*  I numeri reali con rappresentazione a virgola mobile vengono memorizzati all'interno della memoria centrale
 *  seguendo lo standard IEE 754. Tale standard stabilisce l'allocazione di un numero finito e costante di byte
 *  a seconda del tipo di precisione scelta.
 *
 *  L'importanza della sequenza di bit varia in base all'architettura del computer che si sta utilizzando: occorre
 *  quindi stabilire se si sta operando con una memoria di tipo litle endian o big endian.
 *  Nel primo caso il bit più significativo (quello di segno, 0->pos. 1->neg.) verrà rappresentato alla fine, nel
 *  secondo caso verrà rappresentato com primo bit. Occore quindi stabilire il contenuto del primo byte.
 *  Per far ciò si può immaginare che un numero float (32 bit) venga salvato come una sequenza di 4 char (8 bit
 *  ciascuno), e che il contenuto di ogni char sia un numero compreso tra 0 e 255.
 *  Se inserendo un numero positivo di cui si conosce la codifica ci si accorge che il primo byte è 0, allora la
 *  memoria è di tipo big endian, altrimenti sarà little endian.
 *
 *  Per la stampa della codifica basta prendere i 4 numeri e convertirli in binario tramite un algoritmo di
 *  cambiamento di base
 */
#include <stdio.h>

// Funzione di controllo del tipo di endian
/* Il numero utilizzato per il controllo viene passato lla funzione by reference.
 *
 * Per controllare il primo byte si utilizza l'operazione di cast tra puntatori, in particolare
 * si converte un puntatore a int ad un puntatore a unsigned char. Il puntatore ad unsigned char
 * permette di accedere al primo dei byte con i quali viene effettivamnete codificato il numero reale.
 * Se il primo byte, quindi *p == 0 (o equivalentemente p[0]==0 grazie alla dualità puntatore-vettore) allora
 * la memoria utilizza una codifica bigEndian, altrimenti usa la little-endian.
 *
 * I byte successivi, identificati da p[1], p[2]... contengono dei numeri compresi tra 0 e 255 in complemento a 2.
 */
int checkEndian(int *n){
    unsigned char* p;       /* Dichiarazione del puntatore ad unsigned char
                             */
    p=(unsigned char* )n;   /* Conversione da punt. a intero a punt. a char (operrazione di cast tra
                             * puntatori).*/
    if(p[0]==0) {
        printf("Architettura Big endian.\n");
        return 1;
    }
    else if(p[0]==1) {
        printf("Architettura Little endian.\n");
        return 0;
    }
}
// Funzione di stampa della codifica binaria del numero
/* Il puntatore a char *p, equivalente a p[0], punta al primo byte della rappresentazione,
 * e quindi permette di ottenere delle informazioni sul tipo di endian da utilizzare. Consente inoltre di
 * capire come bisnoga procedere nnela lettura dei byte successivi:
 *      1- se si utilizza una memoria big-endian, allora p[0] punta il primo byte e quindi i successivi andranno
 *         letti e codificato da sinistra verso destra; gli indici andranno da 0 fino a sizeof(af)-1
 *         (ricordando che sizeof(af) restituisce come risultato il numero di byte occupati dalla variabile).
 *      2- se si utilizza una memoria little-endian, p[0] punta all'ultimo byte, e quindi i successvi andranno
 *         letti e codificati  da destra verso sinistra;gli indici andranno da sizeof(af)-1 fino a 0.
 *
 * La codifica in binario viene quindi eseguita sui singoli byte, identificati da p[_].
 * Per la codifica in binario si è utilizzato un algoritmo visto a lezione, il quale permette di convertire un
 * numero senza salavare il resto delle divisiono in un vettore.
 *
 * oss: alla funzione viene passato il numero da codificare tramite puntatore a void; poichè, occorre accedere
 * ai vari byte tramite puntatore a char, è necessaria un'operazione di cast.
 */
void stampaCodifica (void *p, int size, int bigEndian){
    int i,j,esp,zeri,stampati=0,spazio,start=0;
    unsigned char* c;
    c=(unsigned char*)p;
    switch (size){                  /* Questo switch viene utilizzato per decidere dopo quanti bit occorre
                                     * stamapre lo spazio che separa la mantissa dall'esponente. La variabile
                                     * spazio inidica il numero di bit da stampare prima di visualizzare lo spazio,
                                     * includendo il bit di segno; per questo motivo il numero di bit risulta
                                     * maggiorato di 1 rispetto al numero di bit a disposizione dell'esponente.
                                     *
                                     * Il numero del 'case' è proprio uguale a sizeof(af)*/
        case 4: spazio=9; break;    /* float: 1 bit segno, 8 bit esponente*/
        case 8: spazio=12; break;   /* double: 1 bit segno, 11 bit esponente*/
        case 12: spazio=16; bigEndian?(start=2):(size-=2); break;   /*  long double: 1 bit segno, 15 bit esponente.
                                                    * Per il long double occorre gestire anche il padding, ovvero
                                                    * i primi 2 byte uguali a 0 prima del bit di segno. Per far ciò
                                                    * si utilizza un'espressione condizionale che permette di stabilire
                                                    * se eliminare i primi 2 byte o gli utlimi 2, a seconda
                                                    * dell'endian trovato. Se big-endian occorre aliminare i primi 2
                                                    * e quindi iniziare la codifica da p[2]; se little-endian
                                                    * bisogna eliminare gli ultimi 2, e quindi si deve iniziare la
                                                    * codifica da p[sizeof(af)-2].*/
    }
    /* For utilizzato per codificare tutti i byte del numero.
     * Per stabilire le condizioni iniziali viene utilizzata un'espressione condizionale
     */
    for(bigEndian?(i=start):(i= size-1) ; bigEndian?(i< size):(i>=0) ; bigEndian?(i++):(i--)){
        for (j=1,esp=1; 2*esp<=c[i]; esp=esp*2,j++);    /* Si determina la potenza di 2 maggiore
                                                         * del numero da codificare */
        c[i]?(zeri=8-j):(zeri=8);       /* Controllo che il byte in esame non sia 0.
                                         * Se è 0, allora devo stampare 8 volte 0, ovvero un byte nullo;
                                         * se è diverso da zero, il numero di 0 da stampare è uguale ad 8 meno
                                         * l'esponente della potenza di 2 subito maggior del numero af.*/
        for(int j=0;j<zeri;j++) {       /* Stampo gli zeri necessari
                                         */
            printf("0");
            stampati++;                 /* Aumento il contatore dei bit di codifica stampati
                                         */
            if(stampati==spazio || stampati==1)     /* Se il numero di bit stampati è 1, alloro si visalizza uno
                                                     * spazio per distanziare l'esponente dal bit di segno; se il
                                                     * numero di stampati è uguale alla variabile spazio occorre
                                                     * stampare uno spazio per distaccare esponente e mantissa*/
                printf("\t");
        }
        while (esp > 0) {       /* Algoritmo di codifica binaria con controllo per gli spazi
                                 */
            if (esp <= c[i]) {
                printf("1");
                stampati++;
                c[i] = c[i] - esp;
            } else {
                printf("0");
                stampati++;
            }
            if(stampati==spazio || stampati==1)
                printf("\t");
            esp = esp / 2;
        }
    }
    printf("\n");
}


int main(){
    int num=1,bigEndian;    /* num=1 è il numero utilizzato per controllare il tipo di endian, in quanto si conosce
                             * la sua codifica: se il primo byte è 0, allora è big-endian, altrimenti sarà little.
                             * bigEndian è una vriabile booleana: vale 0 se la memoria è big-endian, 1 se è little. */
    float af;               /* dichiarazione del numero di tipo float (precisione singola)*/
    double ad;              /* Dichiarazione del numero di tipo double (doppia precisione)*/
    long double ald;        /* Dichiarazione del numero di tipo long double (tripla precisione)*/

    bigEndian=checkEndian(&num);        /* Si richiama la funzione di check dell'endian*/
    printf("Inserisci un numero razionale: ");
    scanf("%f", &af);
    ad = (double) af;                   /* Conversione da float a double*/
    ald = (long double) af;             /* Conversione da flat a long double*/
    printf("\nType\t\tS\tE\t\tM\n");
    printf("Float\t\t");
    stampaCodifica((void *) &af, sizeof(af), bigEndian);        /* Codifica del float*/
    printf("Double\t\t");
    stampaCodifica((void *) &ad, sizeof(ad), bigEndian);        /* Codifica del double*/
    printf("Long Double\t");
    stampaCodifica((void *) &ald, sizeof(ald), bigEndian);      /* Codifica del long double*/



    return 0;
}