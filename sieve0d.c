#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define mask(bit) (1 << (bit)%8)
#define getbit(bit) (buf1[bit >> 3] & (mask(bit)))
#define setbit(bit) buf1[bit >> 3] |= (mask(bit))
#define T unsigned long
#define C unsigned char

#define wheel(i) (3*(i+2)-(i+2)%2-1)
#define antiwheel(i) ((i+2)/3-2)
#define next(i) (2*i-(2*i+1)/3)

// debug
#define print(a) printf("%zu ", a)
#define prints(s) printf("%s", s)
#define endl printf("\n");
#define tb printf("\t");

int main(int argc, char** argv){
    T i, j, d, prime, dprime, count = 0, n = atoll(argv[1]);
    T sqr = sqrt((double)n), bufsize = antiwheel(n);
    
    C* buf1 = (C*) calloc(bufsize/8, sizeof(C));

    for(i = 0; i <= antiwheel(sqr); i++)
        if(!getbit(i)){ 
            prime = wheel(i);
            dprime = 2*prime;
            d = dprime - next(prime);
            for(j = i + dprime - d; wheel(j) + dprime < n; j += dprime){
                setbit(j);
                setbit(j+d); 
            }
            
            if(wheel(j) < n)
                setbit(j);
        }
    
    for(i = 0; i < bufsize; i++)
        if(!getbit(i))
            count++;

    printf("%zu\n", count + 2);
    return 0;
}

/* 
    3 1000 168
    4 10000 1229
    5 100000 9592
    6 1000000 78498
    7 10000000 664579
    8 100000000 5761455
    9 1000000000 50847534          8.87s
   10 10000000000 455052511         2m33   (cloud9)
   11 100000000000 4118054813 
   12 1000000000000 37607912018
   13 10000000000000 346065536839
*/
