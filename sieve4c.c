#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstdlib>

#define getbit(buf, bit) ((buf)[bit >> 3] &  (1 << ((bit)%8)))
#define setbit(buf, bit)  (buf)[bit >> 3] |= (1 << ((bit)%8))

void markBlock(char* buf2, long int low, long int high, long int prime, int id){
	long int j, first = low - low%prime;
	
	if(first<low)
			first += prime;

	if(first%2 == 0)
			first += prime;

	if(first <= high){
		j = (first - low)/2;
		for(; j <= (high-low)/2; j += prime)
				setbit(buf2, j);
	}
};


int64_t Sieve(int64_t n, int id, int p){
    int64_t 
        low, high, blockSize,
        i, j, prime, 
        count = 0;
    
    int64_t sqr = sqrt((double)n);
    int64_t bufsize2, bufsize = (sqr-3)/2+1;

    blockSize = (n-sqr)/p;
    low = sqr + blockSize*id + 1;
    high = low + blockSize - 1;
    
    
    if(id == p-1)
        high += (n-sqr)%p;
        
    if(low % 2 == 0) low ++;
    
    bufsize2 = (high-low)/2+1;
    
    char* buf1 = (char*) calloc(bufsize/8+1, sizeof(char));
    char* buf2 = (char*) calloc(bufsize2/8+1, sizeof(char));
    
    for(i = 0; i <= (sqr-3)/2; i++){
        if(getbit(buf1, i)) continue;
        prime = i*2+3;
        for(j = i+prime; j < bufsize; j += prime)
            setbit(buf1, j); 

        if(id == 0) 
            count++;

        markBlock(buf2, low, high, prime, id);
    }

    for(i = 0; i < bufsize2; i ++)
        if(!getbit(buf2, i))
            count++;

    return count;
}

int main(int argc, char** argv){
	int64_t n = atol(argv[1]);
    int64_t count, globalSum = 0;
    int id, p;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Barrier(MPI_COMM_WORLD);
    
    count = Sieve(n, id, p);

    MPI_Reduce(&count, &globalSum, 1, MPI_UNSIGNED, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    
    if(id == 0) printf("%lld\n", (long long) globalSum + 1);
    
    MPI_Finalize();
    return 0;
}

/* 
    3 1000 168        0.68s
    4 10000 1229        0.67s
    5 100000 9592        0.63s
    6 1000000 78498        0.68s
    7 10000000 664579        0.77s
    8 100000000 5761455        1.33s
    9 1000000000 50847534        17.68s user, 14.3 real (!)
   10 10000000000 455052511       
   11 100000000000 4118054813     
   12 1000000000000 37607912018   
   13 10000000000000 346065536839 
*/
