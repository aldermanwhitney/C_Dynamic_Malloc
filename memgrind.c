#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/**This file will include the memory test
 *To test our functions for mymalloc() and myfree()
 from mymalloc.c
 */

void testA(){

struct timeval start_time;
gettimeofday(&start_time, NULL);
printf("start_time in seconds: %ld\n", start_time.tv_sec);
printf("start_time in microseconds: %ld\n", start_time.tv_usec);


for (int i = 0; i<120; i++){
char *ptr = malloc(1);
free(ptr);
}	
	


struct timeval end_time;
gettimeofday(&end_time, NULL);
printf("end_time: %ld\n", end_time.tv_sec);
printf("end_time in microseconds: %ld\n", end_time.tv_usec);

printf("elapsed_time: %ld\n", end_time.tv_sec-start_time.tv_sec);
printf("elapsed_time in microseconds: %ld\n", end_time.tv_usec-start_time.tv_usec);
}



int main(int argc, char **argv){

testA();
return 0;
}
