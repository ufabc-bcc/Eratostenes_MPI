#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstdlib>

#define getbit(bit) (buf1[bit >> 3] & (1 << ((bit)%8)))
#define setbit(bit) buf1[bit >> 3] |= (1 << ((bit)%8))

int main(int argc, char** argv){
    int64_t 
        i, j, prime, 
        count = 0, 
        n = atol(argv[1]);
    
    int64_t sqr = sqrt((double)n),
            bufsize = (n-3)/2+1;
    char* buf1 = (char*) calloc(bufsize/8, sizeof(char));

    for(i = 0; i <= (sqr-3)/2; i++)
        if(!getbit(i)){ 
            prime = i*2+3;
            for(j = i+prime; j < bufsize; j += prime)
                setbit(j); 
        }

    for(i = 0; i < bufsize; i ++)
        if(!getbit(i)){
            count++;
        }

    printf("%lld\n", (long long)count + 1);
    return 0;
}


/* 
    3 1000 168
    4 10000 1229
    5 100000 9592
    6 1000000 78498
    7 10000000 664579
    8 100000000 5761455          1.49S 
    9 1000000000 50847534          16.2s
   10 10000000000 455052511
   11 100000000000 4118054813 
   12 1000000000000 37607912018
   13 10000000000000 346065536839
*/
