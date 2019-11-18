//
// Created by rosar on 17/11/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 20

int Conta(char S[SIZE],int n);

int main() {
    FILE *fp;
    char nome[SIZE],parola[SIZE];
    int N,lung,cont=0;
    //  Richiesta del nome del file
    printf("Nome del file (no.txt): ");
    scanf("%s",nome);
    // Il nome del file sarà seguito da un .txt, quindi occorre aggiungere questa informazione
    strcat(nome,".txt");
    //  Richiesta della lunghezza della sottostringa cercata
    printf("Lunghezza sottostringa: ");
    scanf("%d",&lung);
    // Apertura del file
    fp=fopen(nome,"r");
    //  Controllo che il file venga aperto correttamente, e quidi che il file sia correttamente puntato
    if(fp==NULL)
        return (EXIT_FAILURE);
    fscanf(fp,"%d",&N);
    while(cont<N){
        fscanf(fp,"%s",parola);
        printf("Parola [%d]: %s \n",cont,parola);
        Conta(parola,lung);
        printf("\n");
        cont++;
    }
    printf("Fine file.");
    fclose(fp);
    return 0;
}

int Conta(char S[SIZE],int n){
    char M[SIZE][n],ch;
    int vocali;

    /*  Scompongo la stringa di caratteri in una matrice M
     *  M avrà al massimo 'SIZE' righe e 'n' colonne
     *  Effettivamente M avrà strlen(s)-n righe */
    for(int i=0;i<strlen(S)-n;i++){
        for(int j=0;j<n;j++)
            M[i][j]=S[i+j];
    }

    /* Eseguo i controlli per la stampa */
    for(int i=0;i<strlen(S)-n;i++){
        vocali=0;
        for(int j=0;j<n;j++){
            ch=tolower(M[i][j]);
            if(ch=='a' || ch=='e' || ch=='i' || ch=='o' || ch=='u' )
                vocali++;
        }
        //  Stampo la sottostringa se vocali>=2
        if(vocali>=2) {
            for (int j = 0; j < n; j++)
                printf("%c", M[i][j]);
            printf("\n");
        }
    }

}
