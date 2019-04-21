#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define C signed char
#define T unsigned long
#define MIN(a,b) a<b?a:b
#define MAX(a,b) a>b?a:b

#define print(a) printf("%zu ", a)
#define endl printf("\n")
#define tb printf("\t")
#define hr printf("\n-----------------------------\n")
#define dot printf(".")
#define hashtag printf("#")
#define sep printf("| ")
#define nan printf(". ")
#define printtb(s) print(s); tb

#define wheel(i) (3*(i+2)-(i+2)%2-1)
#define antiwheel(i) ((i+2)/3-2)
#define next(i) (2*i-(2*i+1)/3)

#define test(bit) ((buf2)[bit >> 3] &  (1 << ((bit)%8)))
#define set(bit)  (buf2)[bit >> 3] |= (1 << ((bit)%8))

#define test1(bit) ((buf1)[bit >> 3] &  (1 << ((bit)%8)))
#define set1(bit)  (buf1)[bit >> 3] |= (1 << ((bit)%8))


T SQR(T n){
    T menor, maior;
    if(n < 0) n = 0;
    if(n < 2) return n;
    menor = SQR(n >> 2) << 1;
    maior = menor + 1;
    if(maior * maior > n) 
        return menor;
    else
        return maior;
}

void markSqrBuf(C *buf1, T i, T sqr){
    T j, d, dprime, prime;

    // o velho dilema: fazer um codigo legivel ou um codigo compacto?
    // Esse loop e o if que o segue sao fruto de escovacao de bits (explico no relatorio)
    // na verdade, sao dois loops, um pra cada coluna de candidatos a primo.
    // o tratamento das colunas 6k-1 e 6k+1 faz parte da estrategia wheel
    // muito usada por um tal Pritchard em 1983. Ele recomenda wheels dinamicos
    // que variam de acordo com o tamanho de n, mas isso foge das minhas capacidades. (atuais)

    prime = wheel(i);
    dprime = 2*prime;
    d = dprime - next(prime);
    for(j = i + dprime - d; wheel(j) + dprime < sqr; j += dprime){
        set1(j);
        set1(j+d);
    }
    if(wheel(j) < sqr)
        set1(j);
}


void markSegBuf(C* buf2, T x, T y, T p){
	T first, seg = y-x;
	T i, j, prime = wheel(p);
	T low = wheel(x), high = wheel(y), dprime = 2*prime;
	
	first = antiwheel(prime*prime);
	while(first < x)
	    first+=dprime;
 	for(i = first; i <= y; i += dprime)
	    set(i-x);

	// Olha a treta pra encontrar o primeiro 6k+5 maior que primo*primo
	p++;
	first = antiwheel(prime*prime + (p%2?p:2*p)*6+4);
	while(first < x)
	    first+=dprime;
	for(i = first; i <= y; i += dprime)
	    set(i-x);
}

T trabalho(T n, T low, T high){
    T i, j, b, sqr = SQR(n), seg = high-low; 
    C* buf1 = (C*) calloc(sqr/20, 1);
    C* buf2 = (C*) calloc(seg/8, 1);
    
    T count = 0;
    for(i = 0; i <= antiwheel(sqr); i++)
        if(!test1(i)){
            markSqrBuf(buf1, i, sqr);
            markSegBuf(buf2, low, high, i);
        }

    for(i = 0; i <= seg; i++)
        if(!test(i))
            count++;
        
    free(buf1);
    free(buf2);
    return count;
}

int main(int argc, char** argv){
    T m, i, j, k, low, high, primo, sum = 0, seg, count, p = atoll(argv[1]), n = atoll(argv[2]);    

    // antiwheel faz o contrario de wheel. 
    // percorre o crivo e retorna o indice do numero
    // considerando duas das seis colunas do crivo. 
    // Isso ajuda a guardar uma quantidade bem menor de dados. 
    m = antiwheel(n-1);
    
    // dividir o trabalho... da trabalho.
    // deveria ter uma disciplina so pra isso.
    sum = 0;
    count = 0;
    seg = (m+p)/p;
    for(low = 0; low < m; low += seg){
        high = MIN(low + seg - 1, m);
        count = trabalho(n, low, high);
        sum += count;
    }
    print(sum+2);
    return 0;
}


/* 
    3 1000 168
    4 10000 1229
    5 100000 9592
    6 1000000 78498
    7 10000000 664579
    8 100000000 5761455        
    9 1000000000 50847534      (9.484s cloud9) (31.537s chromebook)
   10 10000000000 455052511
   11 100000000000 4118054813 
   12 1000000000000 37607912018
   13 10000000000000 346065536839
*/
