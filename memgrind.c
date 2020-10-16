#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/**This file will include the memory test
 *To test our functions for mymalloc() and myfree()
 from mymalloc.c
 */


/**Test A: Malloc one byte and immediately free it, 120 times
 *returns int - the time in microseconds the entire operation required
 */
int testA(){

struct timeval start_time;
gettimeofday(&start_time, NULL);
//printf("start_time in seconds: %ld\n", start_time.tv_sec);
//printf("start_time in microseconds: %ld\n", start_time.tv_usec);


for (int i = 0; i<120; i++){
char *ptr = malloc(1);
free(ptr);
}	
	


struct timeval end_time;
gettimeofday(&end_time, NULL);
//printf("end_time: %ld\n", end_time.tv_sec);
//printf("end_time in microseconds: %ld\n", end_time.tv_usec);

//printf("elapsed_time in seconds: %ld\n", end_time.tv_sec-start_time.tv_sec);
printf("Test A: elapsed_time in microseconds: %ld\n", end_time.tv_usec-start_time.tv_usec);

return ((end_time.tv_usec)-(start_time.tv_usec));
}


/**Test B: Malloc 1 byte and add to an array 120 times
 *Then, free bytes from the array one by one
 *returns int - the number of microseconds of the entire process
 */
int testB(){

char *ptrarray[120];

struct timeval start_time;
gettimeofday(&start_time, NULL);

for (int i = 0; i<120; i++){
char *ptr = malloc(1);
ptrarray[i]=ptr;
}

for(int j = 0; j<120; j++){
free(ptrarray[j]);
}


printf("%s", ptrarray[0]); //this line is here temporarily for compilation

struct timeval end_time;
gettimeofday(&end_time, NULL);


printf("Test B: elapsed_time in microseconds: %ld\n", end_time.tv_usec-start_time.tv_usec);
return ((end_time.tv_usec)-(start_time.tv_usec));
}



void printruntimes(int runtime[50][2]){

printf("Test A\tTest B\n");
for (int i = 0; i<50; i++){

for (int j=0; j<2; j++){
printf("%d\t", runtime[i][j]);
}
printf("\n");	
}

}

void printmeanruntimes(int runtime[50][2]){

double testASum = 0;
double testBSum = 0;
	
for (int i = 0; i<50; i++){

for (int j=0; j<2; j++){
if(j==0){
testASum += runtime[i][j];
}
else{
testBSum += runtime[i][j];
}
}	
}

printf("Average Runtimes:\n");
printf("Test A\tTest B\n");
printf("%.3f\t", testASum/50);
printf("%.3f\t", testBSum/50);

}


int main(int argc, char **argv){

int runtime[50][2]; //initializes 50 row, 2 column array to store runtimes - will increase column size for every new test

for(int i = 0; i<50; i++){

	for (int j = 0; j<2; j++){

	//column 0, Test A runtimes	
	if (j==0){
	runtime[i][j]=(testA());
	}
	//column 1, Test A runtimes	
	else{
	runtime[i][j]=(testB());
	}
	}
}

printruntimes(runtime);
printmeanruntimes(runtime);
return 0;
}
