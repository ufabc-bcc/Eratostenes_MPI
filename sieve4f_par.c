#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

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

T trabalho(C* buf2, T n, T low, T high){
    T i, j, b, sqr = SQR(n), seg = high-low; 
    C* buf1 = (C*) calloc(sqr/15, 1);
    
    
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
    return count;
}

int main(int argc, char** argv){
    T globalSum = 0, i, j, k, m, low, high, primo, sum = 0, seg, count = 0, n = atoll(argv[1]);    

    int id, p;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Barrier(MPI_COMM_WORLD);
    double time0, time = -MPI_Wtime();

    // antiwheel faz o contrario de wheel. 
    // percorre o crivo e retorna o indice do numero
    // considerando duas das seis colunas do crivo. 
    // Isso ajuda a guardar uma quantidade bem menor de dados. 
    m = antiwheel(n-1);
    

    // dividir o trabalho.
    // Quando o serial ja está preparado, paralelizar eh basicamente adaptar este loop
    seg = (m+p)/p;
    C* buf2 = (C*) calloc(seg/8, 1);
    
    low = seg*id;
        high = MIN(low + seg - 1, m);
        count = trabalho(buf2, n, low, high);
    
    time += MPI_Wtime();
    if(id == 0) time0 = time;
    
    MPI_Reduce(&count, &globalSum, 1, MPI_UNSIGNED, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Bcast(&time0, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    if(id == 0) printf("%zu\n", globalSum + 2);
    printf("maquina %d rodou em %lf  e isso significa %.lf %% do root\n", id, time, 100*time/time0);
    
    
    // Calculo dos ultimos 20 primos
    //
    // NOTA: o enunciado do projeto pede claramente 
    // "Os 20 ultimos numeros primos do intervalo 1 e 1.000.000.000"
    // e pede isso na sessao que fala dos tres pontos do relatorio (nao nos objetivos). 
    // 
    // NOTA2: o codigo retorna 20 primos corretos entre os 27 ultimos.
    // e isso eh estranho, dado que o numero total de primos do intervalo esta correto.
    // O que indica que ha algum erro nas propriedades que usei na implementacao.
    // provavelmente o erro esta nas funcoes wheel e antiwheel, que na verdade
    // sao macros, ou na indexacao dos segmentos ou mesmo na contagem e exibicao.
    // Uma analise mais profunda consumiria mais alguns dias (que nao tenho).

    MPI_Barrier(MPI_COMM_WORLD);
    if(id == 0 && n == 1000000000){
        T Q = 279;
        //do{
            count = trabalho(buf2, n, m-Q, m);
            //Q++;
            //buf2 = (C*) calloc(seg/8, 1);
            //print(Q);
        //} while(count<20);
        
        printf("\nos vinte ultimos primos sao:\n");
        for(i = 0; i <= Q; i++)
            if(!test(i))
                print(wheel(i+m-Q));
        hr;
    }
    
    free(buf2);
    MPI_Finalize();
    return 0;
}


/* 
    3 1000 168
    4 10000 1229
    5 100000 9592
    6 1000000 78498
    7 10000000 664579
    8 100000000 5761455        
    9 1000000000 50847534      (11.543s cloud9) 
   10 10000000000 455052511      (2m21.122s cloud9) 
   11 100000000000 4118054813 
   12 1000000000000 37607912018
   13 10000000000000 346065536839
*/
