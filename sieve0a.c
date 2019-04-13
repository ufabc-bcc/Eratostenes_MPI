#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv){
    long int 
        i, j, prime, 
        count = 0, 
        n = atol(argv[1]);
    
    long int sqr = sqrt((double)n);
    char* buf1 = (char*) calloc((n-3)/2+1, sizeof(char));

    for(i = 0; i <= (sqr-3)/2; i++)
        if(!buf1[i]){ 
            prime = i*2+3;
            for(j = i+prime; j <= (n-3)/2; j += prime)
                buf1[j] |= 1; // remove multiples
        }

    for(i = 0; i < (n-3)/2; i ++)
        if(!buf1[i]){
            //printf("%zu ", i*2+3);
            count++;
        }

    //printf("\n");
    printf("%zu\n", count + 1);
    return 0;
}


/* 
    3 1000 168
    4 10000 1229
    5 100000 9592
    6 1000000 78498
    7 10000000 664579
    8 100000000 5761455     2.47s
    9 1000000000 50847534    25.559s
   10 10000000000 455052511
   11 100000000000 4118054813 
   12 1000000000000 37607912018
   13 10000000000000 346065536839
*/
