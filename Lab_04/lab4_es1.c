//
// Created by rosar on 17/11/2019.
//

#include <stdio.h>

typedef enum {
    uguali, a_gcd, b_gcd, evens, a_odd_b_even, a_even_b_odd, odds, error
}casi;

casi GestioneCasi(int a, int b){
    if(a==b)
        return uguali;
    else if(a%b==0)
        return b_gcd;
    else if (b%a==0)
        return a_gcd;
    else if(a%2==0 && b%2==0)
        return evens;
    else if(a%2!=0 && b%2==0)
        return a_odd_b_even;
    else if(a%2==0 && b%2!=0)
        return a_even_b_odd;
    else if(a%2!=0 && b%2!=0)
        return odds;
    else
        return error;
}

int gcd(int a, int b){
    int temp;
    if(b>a){
        temp=a;
        a=b;
        b=temp;
    }
    switch (GestioneCasi(a,b)) {
        case uguali: return a;
        case a_gcd: return a;
        case b_gcd: return b;
        case evens: return (2*gcd(a/2,b/2));
        case a_odd_b_even: return gcd(a,b/2);
        case a_even_b_odd: return gcd(a/2,b);
        case odds: return (gcd((a-b)/2,b));
        case error: printf("Errore!"); return 0;
    }
}

int main() {
    int a,b,mcd;

    printf("int a: ");
    scanf("%d",&a);
    printf("int b: ");
    scanf("%d",&b);

    // Det. MCD
    mcd=gcd(a,b);
    printf("gcd: %d\n",mcd);

    return 0;
}