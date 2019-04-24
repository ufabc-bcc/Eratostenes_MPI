#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define C signed char
#define T unsigned long
#define MIN(a,b) (a<b?a:b)
#define MAX(a,b) (a>b?a:b)

#define print(a) printf("%zu ", (a))
#define endl printf("\n")
#define tb printf("\t")
#define hr printf("\n-----------------------------\n")
#define dot printf(".")
#define hashtag printf("#")
#define sep printf("| ")
#define nan printf(". ")
#define printtb(s) print((s)); tb

#define wheel(i) (3*(i+2)-(i+2)%2-1)
#define antiwheel(i) ((i+2)/3-2)
#define next(i) (2*i-(2*i+1)/3)

#define test(bit) ((buf2)[(bit) >> 3] &  (1 << ((bit)%8)))
#define set(bit)  (buf2)[(bit) >> 3] |= (1 << ((bit)%8))

#define test1(bit) ((buf1)[(bit) >> 3] &  (1 << ((bit)%8)))
#define set1(bit)  (buf1)[(bit) >> 3] |= (1 << ((bit)%8))

/*

        Por favor, não use esta versão para avaliação, 
    mas eu não podia deixar de postar a melhoria que eu estava esse tempo todo tentando fazer
    (e que, infelizmente, não consegui implementar a tempo), que me possibilitou passar de 10¹¹  
    em incríveis 8m13s na plataforma cloud9 
    (leve em consideração o monte de limitações impostas pela plataforma). 
    
        Trata-se da última versão serial enviada, com a eliminação dos loops while() da função 
    markSegBuf() que provocavam gargalo, adicionados a boa utilização do cache mediante o 
    parâmetro p no valor de 13050, que nos possibilita trabalhar com segmentos de 2554240 bits
    (319280 bytes), otimizando a busca nos 10¹¹ números pelos 4 118 054 813 primos!
    
        ps: esse codigo eh pra pedir perdao por todo erro cometido no anterior
    nesse, detectei falhas grotescas de que me envergonho. 


*/


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
    T j, d, dprime, prime, indice_p_x_next_p, indice_pxp;

    // sao dois loops, um pra cada coluna de candidatos a primo.
    // um deles começa no indice de p² e o outro começa no indice do p*(next p)
    // o tratamento das colunas 6k-1 e 6k+1 faz parte da estrategia wheel
    // muito usada por um tal Pritchard em 1983. Ele recomenda wheels dinamicos
    // que variam de acordo com o tamanho de n, mas isso foge das minhas capacidades. (atuais)
    // a explicacao a seguir o Pritchard nao da. eu tive que bolar. 
    /*
    
    pra entender o que eu to chamando de indice, enumere os numeros dessas colunas
    numa coluna so. 
    
    indice | candidato a primo
         0 | 5
         1 | 7
         2 | 11
         3 | 13
         ...
         ...
         7 | 25
         ...
         ...
        15 | 49
        16 | 53
        17 | 55
        ...
        
        vc tem que marcar todos os multiplos de um dado primo i 
        nessa coluna temos primos 5, 7...  e candidatos a primos, como o 25, o 35
        numeros da forma 6k+-1
        para acessa-los eu uso wheel(i).
        Quando wheel(i) eh um primo, wheel(i+2*primo) eh um multiplo desse primo.
        Entao, se eu encontrar o indice de primo*primo basta marcar a cada 2*primo, 
        ate o limite (sqr nesse caso)
        soh que os multiplos nao estao soh a cada 2*primo a partir de primo*primo.
        eles estao tambem a cada 2*primo a partir de primo*wheel(i+1) que nem sempre eh primo.
        Explicar com texto eh dificil.
        se vc desenhar a coluna e procurar os multiplos vai ver o padrao. 
    
    */
    
    prime = wheel(i);
    dprime = 2*prime;
    indice_pxp = antiwheel(prime*prime);
    indice_p_x_next_p = antiwheel(prime*wheel(i-1));
    
    for(j = indice_pxp; j  <= antiwheel(sqr); j += dprime)
        set1(j);

    for(j = indice_p_x_next_p; j  <= antiwheel(sqr); j += dprime)
        set1(j);
}

void mark(C* buf2, T ii, T x, T y, T p){
    T i, first , dprime = 2*wheel(p);
    
    // calcular o lugar certo 
    //de marcar o primeiro primo nesse segmento
    
    first = antiwheel(ii);
    
    if(first < x) 
        first =   x - (x-first)%dprime;
    
    if(first < x) // dois ifs com a mesma condicao? first nao eh o mesmo. 
        first += dprime;
    
    for(i = first-x ; i <= y-x; i += dprime) 
        set(i);

}

void markSegBuf(C* buf2, T x, T y, T p){
	T i, j, prime = wheel(p), next = wheel(p+1);
	// para cada uma das colunas 6k+1 e 6k-1
	mark(buf2, prime*prime, x, y, p);
	mark(buf2, prime*next, x, y, p);
}

T trabalho(T sqr, T low, T high){
    T i, j, b, seg = high-low; 
    T sqrBuf = antiwheel(sqr);
    C* buf1 = (C*) calloc(sqrBuf/8, 1);
    C* buf2 = (C*) calloc(seg/8, 1);
    
    T count = 0;
    for(i = 0; i <= sqrBuf; i++)
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
    T m, i, j, k, low, high, primo, sum = 0, seg, count;    
    
    if(argc<2) {
        printf("\n\nusage: %s p n\n onde:\n\t p: numero de segmentos. Em testes no cloud9 320kb resultou melhor aproveitamento de cache \n\t n: limite numerico para a quantidade de primos\n\n\n", argv[0]); 
        exit(0);
    }
    
    T p = atoll(argv[1]), n = atoll(argv[2]);

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
    
    printf ("Tamanho do segmento: %.2fkb\n", (double)seg/8000); endl;
    
    for(low = 0; low < m; low += seg){
        high = MIN(low + seg - 1, m);
        count = trabalho(SQR(n), low, high);
        sum += count;
    }
    
    printf("Numero de primos menores que %zu: %zu\n\n", n, sum+2);
    return 0;
}


/* 
    3 1000 168
    4 10000 1229
    5 100000 9592
    6 1000000 78498
    7 10000000 664579
    8 100000000 5761455        
    9 1000000000 50847534      (3.6s cloud9)
   10 10000000000 455052511      (36.4s cloud9, p = 1305) (28.96s lab 503)
   11 100000000000 4118054813       (6m44s cloud9, p = 13050) (5m13s lab 503)
   12 1000000000000 37607912018
   13 10000000000000 346065536839
*/
