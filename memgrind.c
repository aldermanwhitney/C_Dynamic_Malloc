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
//printf("Test A: elapsed_time in microseconds: %ld\n", end_time.tv_usec-start_time.tv_usec);

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


//printf("%s", ptrarray[0]); //this line is here temporarily for compilation

struct timeval end_time;
gettimeofday(&end_time, NULL);


//printf("Test B: elapsed_time in microseconds: %ld\n", end_time.tv_usec-start_time.tv_usec);
return ((end_time.tv_usec)-(start_time.tv_usec));
}

/*Gets a random number between 0 and 1, 
to randomly malloc and free pointers 240 times.
Mallocing not allowed after 120 mallocs are made
Freeing not allowed if no pointers to free
returns int - the number of microseconds of the entire process 
*/
int testC(){

struct timeval start_time;
gettimeofday(&start_time, NULL);
 
 int numberMalloced=0;
 char *ptrarray[120];
 int numberFreed=0;
 int totalMallocs=0;

 for(int i = 0; i < 240; i++){

 //choosing random between 0 and 1 
 int random = rand()%2;
 //  printf("random number is %d\n", random);
 if(totalMallocs==120 && numberFreed!=120){
   random=1;
 }
 // printf("random number is %d\n", random);
 if(random==0){
   //malloc if random is 0
   char *ptr = malloc(1);
   ptrarray[numberMalloced] = ptr;
   numberMalloced++;
   totalMallocs++;
 }
 else if(random==1){
   //free if random is 1
   if(numberMalloced!=0 && ptrarray[numberMalloced-1]!=NULL){
   free(ptrarray[numberMalloced-1]);
   numberFreed++;
   numberMalloced--;
   }
   else{
     //     numberFreed++;
     // printf("nothing to free\n");
   }
 }
 }

 printf("Number of mallocs %d, number of frees %d\n",totalMallocs,numberFreed);
struct timeval end_time;
gettimeofday(&end_time, NULL);

return ((end_time.tv_usec)-(start_time.tv_usec));
}


void printruntimes(int runtime[50][3]){

printf("Test A\tTest B\tTest C\n");
for (int i = 0; i<50; i++){

for (int j=0; j<3; j++){
printf("%d\t", runtime[i][j]);
}
printf("\n");	
}

}

void printmeanruntimes(int runtime[50][3]){

double testASum = 0;
double testBSum = 0;
double testCSum = 0;
for (int i = 0; i<50; i++){

for (int j=0; j<3; j++){
if(j==0){
testASum += runtime[i][j];
}
 else if(j==1){
testBSum += runtime[i][j];
}
 else if(j==2){
   testCSum += runtime[i][j];
 }
}	
}

printf("Average Runtimes:\n");
printf("Test A\tTest B\tTest C\n");
printf("%.3f\t", testASum/50);
printf("%.3f\t", testBSum/50);
 printf("%.3f\t", testCSum/50);

}


int main(int argc, char **argv){

int runtime[50][3]; //initializes 50 row, 3 column array to store runtimes - will increase column size for every new test

for(int i = 0; i<50; i++){

	for (int j = 0; j<3; j++){

	//column 0, Test A runtimes	
	if (j==0){
	runtime[i][j]=(testA());
	}
	//column 1, Test B runtimes	
	else if(j==1){
	runtime[i][j]=(testB());
	}
	//column 2, Test C runtimes
	else if(j==2){
	  runtime[i][j]=(testC());
	}
	}
}

printruntimes(runtime);
printmeanruntimes(runtime);
return 0;
}
