#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "mymalloc.h" //needed when youre ready to link to mymalloc
//#include "mymalloc.c"

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

 return ((((end_time.tv_usec)-(start_time.tv_usec))*1000000)+((end_time.tv_usec)-(start_time.tv_usec)));
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

 return ((((end_time.tv_usec)-(start_time.tv_usec))*1000000)+((end_time.tv_usec)-(start_time.tv_usec)));
}

/*Gets a random number between 0 and 1, 
to randomly malloc and free pointers 240 times.
Mallocing not allowed after a total of 120 mallocs are made
Freeing not allowed if no pointers to free, frees all pointers malloced
returns int - the number of microseconds of the entire process 
*/
int testC(){

struct timeval start_time;
gettimeofday(&start_time, NULL);
 
 int numberMalloced=0;
 char *ptrarray[120];
 int numberFreed=0;
 int totalMallocs=0;
 int i = 0;
 while(totalMallocs<=120 && i < 240){
 //choosing random between 0 and 1 
 int random = rand()%2;
 //  printf("random number is %d\n", random);
 if(totalMallocs==120 && numberFreed!=120){
   random=1;
 }
 // printf("random number is %d\n", random);
 if(random==1){
   //free if random is 1                                                                                                                                               
   if(numberMalloced!=0 && ptrarray[numberMalloced-1]!=NULL){
   free(ptrarray[numberMalloced-1]);
   numberFreed++;
   numberMalloced--;
   }
   else{
    random=0;
    //printf("nothing to free\n");
   }
 }
 if(random==0){
   //malloc if random is 0
   char *ptr = malloc(1);
   ptrarray[numberMalloced] = ptr;
   numberMalloced++;
   totalMallocs++;
 }
  
 i++;
 }
 // printf("Number of mallocs %d, number of frees %d\n",totalMallocs,numberFreed);
struct timeval end_time;
gettimeofday(&end_time, NULL);

return ((((end_time.tv_usec)-(start_time.tv_usec))*1000000)+((end_time.tv_usec)-(start_time.tv_usec)));
}

/*Mallocs an array byte by byte for 240 bytes and
frees each odd byte in the whole array,
then tries to malloc more than 1 byte in 
a smaller free space i.e in a 1 byte space
tests for splitting the array as malloc and
frees are made and tests for mallocing a block 
bigger than all the available free blocks
*/
int testD(){
struct timeval start_time;
gettimeofday(&start_time, NULL);

char *ptrarray[260];
//printf("size of ptr array%ld\n", sizeof(ptrarray));
 int mcounter=0, fcount=0, realloc=0;
 for(int i = 0; i < 260; i++){
   char *ptr = malloc(1);
   if(ptr!=NULL){
   ptrarray[i]=ptr;
   mcounter++;
   }
 }

 for(int i = 0; i<260; i++){
   if(ptrarray[i]!=NULL && i%2==1){
     free(ptrarray[i]);
     fcount++;
   }
 }

 for(int i = 0; i < 260; i++){
   //char *ptr = malloc(2);
   if(i%2==1){
     char *ptr = malloc(2);
     if(ptr!=NULL){
     ptrarray[i]=ptr;
     realloc++;
     }
   }
 }
 /* 
 for(int i = 0; i<240; i++){
   if(ptrarray[i]!=NULL){
     free(ptrarray[i]);
   }
 }
 */
 printf("malloc count %d, freed count %d, realloc count %d, size of array %ld\n",mcounter,fcount,realloc,sizeof(ptrarray)); 
struct timeval end_time;
gettimeofday(&end_time, NULL);
return ((((end_time.tv_usec)-(start_time.tv_usec))*1000000)+((end_time.tv_usec)-(start_time.tv_usec)));
}

/* allocates 240 bytes and 
frees them in order starting from 120 bytes 
to test for stiching 
of consecutive free blocks in order
to allocate a big chunk of memory in
this stiched block
*/
int testE(){
struct timeval start_time;
gettimeofday(&start_time, NULL);

 char *ptrarray[240];
 for(int i = 0; i < 24; i++){
   char *ptr = malloc(10);
   ptrarray[i]=ptr;
 }

 for(int i = 12; i < 24; i++){
   free(ptrarray[i]);
   if(i>12 && i%3==0){
     char *ptr = malloc(50);
     if(ptr!=NULL){
     ptrarray[i]=ptr;
     }
     else{
       char *ptr = malloc(30);
       ptrarray[i]=ptr;
   }
 }
 }
 
struct timeval end_time;
gettimeofday(&end_time, NULL);
return ((((end_time.tv_usec)-(start_time.tv_usec))*1000000)+((end_time.tv_usec)-(start_time.tv_usec)));

}
/*
int testF(){
struct timeval start_time;
gettimeofday(&start_time, NULL);

//for(int i = 0; i < 120; i++){
   int x;
   free( (int*)x );
 
 char* p;
 //for(int i = 0; i < 120; i++){
     p = (char *)malloc( 200 );
     free(p+10);
 

 // for(int i = 0; i < 120; i++){
    int * x;
    free(x);
  

  p = (char*)malloc(100);
  free(p);
  free(p);

  p = (char *)malloc( 100 );
  free(p);
  p = (char *)malloc( 100 );
  free(p);

 p = (char*)malloc(4096);
 q = (char*)malloc(1);
 p = (char*)malloc(5000);
struct timeval end_time;
gettimeofday(&end_time, NULL);
return ((end_time.tv_usec)-(start_time.tv_usec));
}
*/

void printruntimes(int runtime[50][5]){

printf("Test A\tTest B\tTest C\tTest D\tTest E\n");
for (int i = 0; i<50; i++){

for (int j=0; j<5; j++){
printf("%d\t", runtime[i][j]);
}
printf("\n");	
}

}

void printmeanruntimes(int runtime[50][5]){

double testASum = 0;
double testBSum = 0;
double testCSum = 0;
double testDSum = 0;
double testESum = 0;
for (int i = 0; i<50; i++){

for (int j=0; j<5; j++){
if(j==0){
testASum += runtime[i][j];
}
 else if(j==1){
testBSum += runtime[i][j];
}
 else if(j==2){
   testCSum += runtime[i][j];
 }
 else if(j==3){
   testDSum += runtime[i][j];
 }
 else if(j==4){
   testESum += runtime[i][j];
 }
}	
}

printf("Average Runtimes:\n");
printf("Test A\tTest B\tTest C\tTest D\tTest E\n");
printf("%.3f\t", testASum/50);
printf("%.3f\t", testBSum/50);
printf("%.3f\t", testCSum/50);
printf("%.3f\t", testDSum/50);
printf("%.3f\t", testESum/50); 
}


int main(int argc, char **argv){
  // int *x = malloc(4080);
  // int *y = malloc(1);
int runtime[50][5]; //initializes 50 row, 3 column array to store runtimes - will increase column size for every new test

for(int i = 0; i<1; i++){

	for (int j = 0; j<5; j++){
	 
	//column 0, Test A runtimes	
	if (j==0){
	runtime[i][j]=(testA());
	}
	
	
	//column 1, Test B runtimes	
        if(j==1){
	runtime[i][j]=(testB());
	}
	
	//column 2, Test C runtimes
        if(j==2){
	  runtime[i][j]=(testC());
	  }
	  
	  /*
	//column 3, Test D runtimes
	 if(j==3){
          runtime[i][j]=(testD());
	  }
	  */
	 /*
	//column 4, Test E runtimes
        if(j==4){
          runtime[i][j]=(testE());
        }
	  */
	
	}
}
// printlinkedlist();
 //printf("%ld", sizeof(int));
printruntimes(runtime);
printmeanruntimes(runtime);
return 0;
}
