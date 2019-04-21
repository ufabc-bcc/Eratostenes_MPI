#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv){
    long int i, j, count = 0, n = atol(argv[1]);    
    char* buf = (char*) calloc(n+1, sizeof(char));

    for(i=2; i < n; i++)
        if(!buf[i]){
            for(j = i*i; j <= n; j+=i)
                buf[j] |= 1;
            count ++;
        }
                

    printf("\n%zu\n", count);
    return 0;
}


/* 
    3 1000 168
    4 10000 1229
    5 100000 9592
    6 1000000 78498
    7 10000000 664579
    8 100000000 5761455        4.559s
    9 1000000000 50847534
   10 10000000000 455052511
   11 100000000000 4118054813 
   12 1000000000000 37607912018
   13 10000000000000 346065536839
*/
