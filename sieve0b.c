#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv){
    long int 
        i, j, prime, w, 
        count = 0, 
        n = atol(argv[1]);
    
    long int sqr = sqrt((double)n);
    char* buf1 = (char*) calloc(n+1, sizeof(char));

    for(i = 5, w = 2; i <= sqr; i += w, w = 6-w)
        if(!buf1[i]){ 
            for(j = i*i; j <= n; j += i)
                buf1[j] |= 1; // remove multiples
        }

    for(i = 5, w = 2; i <= n; i += w, w = 6-w)
        if(!buf1[i]){
            //printf("%zu ", i);
            count++;
        }

    //printf("\n");
    printf("%zu\n", count + 2);
    return 0;
}


/* 
    3 1000 168
    4 10000 1229
    5 100000 9592
    6 1000000 78498
    7 10000000 664579
    8 100000000 5761455            3.82s
    9 1000000000 50847534
   10 10000000000 455052511
   11 100000000000 4118054813 
   12 1000000000000 37607912018
   13 10000000000000 346065536839
*/
