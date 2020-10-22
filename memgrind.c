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
double testA(){
	
struct timeval start_time;
gettimeofday(&start_time, NULL);

//printf("testA Begin\n");
//printlinkedlist();
for (int i = 0; i<120; i++){
char *ptr = malloc(1);


//printf("testA After Malloc\n");
//printlinkedlist();
free(ptr);



//printf("testA After Free\n");
//printlinkedlist();
}	

struct timeval end_time;
gettimeofday(&end_time, NULL);


//printf("testA End\n");
//printlinkedlist();
return (double)(((end_time.tv_sec * 1000000) + (end_time.tv_usec)) - ((start_time.tv_sec * 1000000) + (start_time.tv_usec)));
}


/**Test B: Malloc 1 byte and add to an array 120 times
 *Then, free bytes from the array one by one
 *returns int - the number of microseconds of the entire process
 */
double testB(){

char *ptrarray[120];

struct timeval start_time;
gettimeofday(&start_time, NULL);

for (int i = 0; i<120; i++){
char *ptr = malloc(1);
//printf("testB After Malloc\n");
//printlinkedlist();
ptrarray[i]=ptr;
}
//printlinkedlist();
for(int j = 0; j<120; j++){
free(ptrarray[j]);
//printf("testB After Free\n");
//printlinkedlist();
}
//printlinkedlist();
//printf("%s", ptrarray[0]); //this line is here temporarily for compilation

struct timeval end_time;
gettimeofday(&end_time, NULL);

return (double)(((end_time.tv_sec * 1000000) + (end_time.tv_usec)) - ((start_time.tv_sec * 1000000) + (start_time.tv_usec)));
}

/*Gets a random number between 0 and 1, 
to randomly malloc and free pointers 240 times.
Mallocing not allowed after a total of 120 mallocs are made
Freeing not allowed if no pointers to free, frees all pointers malloced
returns int - the number of microseconds of the entire process 
*/
double testC(){

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
 //  printf("Number of mallocs %d, number of frees %d\n",totalMallocs,numberFreed);
struct timeval end_time;
gettimeofday(&end_time, NULL);

return (double)(((end_time.tv_sec * 1000000) + (end_time.tv_usec)) - ((start_time.tv_sec * 1000000) + (start_time.tv_usec)));
}

/*
allocates 50 bytes blocks 62 times which                                                                                                                                                                     
allocates the whole memory array                                                                                                                                                                                
then frees every odd block                                                                                                                                                                                      
After freeing, reallocates 45 bytes at the same position                                                                                                                                                        
from where the 50 bytes were freed. This tests for                                                                                                                                                              
splitting the block when malloc call                                                                                                                                                                   
has a size smaller than available sizeblock
 but available sizeblock 
is not big enough to split into two with one block
of size asked and another block of leftover size 
because leftover size 50-45 < size of metadata so cannot 
split into two blocks and give user a little extra than they need
*/
double testD(){
struct timeval start_time;
gettimeofday(&start_time, NULL);

char *ptrarray[63];
//printf("size of ptr array%ld\n", sizeof(ptrarray));
 int mcounter=0, fcount=0, realloc=0;
 for(int i = 0; i <63 ; i++){
   char *ptr = malloc(50);
   if(ptr!=NULL){
   ptrarray[i]=ptr;
   mcounter++;
   }
 }
 // printlinkedlist();
 
 for(int i = 0; i<63; i++){
   if(ptrarray[i]!=NULL && i%2==1){
     free(ptrarray[i]);
     fcount++;
   }
 }
 // printlinkedlist();
 
 for(int i = 0; i < 63; i++){
   //char *ptr = malloc(2);
   if(i%2==1){
     char *ptr = malloc(45);
     if(ptr!=NULL){
     ptrarray[i]=ptr;
     realloc++;
     }
   }
   }
 // printlinkedlist();
  
 for(int i = 61; i>=0; i--){
   if(ptrarray[i]!=NULL){
     free(ptrarray[i]);
   }
 }
 //  printlinkedlist(); 
 // printf("malloc count %d, freed count %d, realloc count %d, size of array %ld\n",mcounter,fcount,realloc,sizeof(ptrarray)); 
struct timeval end_time;
gettimeofday(&end_time, NULL);

return (double)(((end_time.tv_sec * 1000000) + (end_time.tv_usec)) - ((start_time.tv_sec * 1000000) + (start_time.tv_usec)));
}

/* allocates 50 bytes blocks 62 times which
allocates the whole memory array
then frees every odd block 
After freeing, reallocates 30 bytes at the same position
from where the 50 bytes were freed. This tests for
splitting the block into two when malloc call
has a size smaller than available size and available size
is big enough to break into two blocks, one of the requested 
malloc call and second block of thhe leftover size + size of metadata
*/
double testE(){
struct timeval start_time;
gettimeofday(&start_time, NULL);

 char *ptrarray[63];
//printf("size of ptr array%ld\n", sizeof(ptrarray));                                                                                                                                                           
 int mcounter=0, fcount=0, realloc=0;
 for(int i = 0; i <63 ; i++){
   char *ptr = malloc(50);
   if(ptr!=NULL){
   ptrarray[i]=ptr;
   mcounter++;
   }
 }
 // printlinkedlist();                                                                                                                                                                                          

 for(int i = 0; i<63; i++){
   if(ptrarray[i]!=NULL && i%2==1){
     free(ptrarray[i]);
     fcount++;
   }
 }
 // printlinkedlist();                                                                                                                                                                                          

 for(int i = 0; i < 63; i++){
   //char *ptr = malloc(2);                                                                                                                                                                                     
   if(i%2==1){
     char *ptr = malloc(30);
     if(ptr!=NULL){
     ptrarray[i]=ptr;
     realloc++;
     }
   }
   }
 // printlinkedlist();                                                                                                                                                                                           
  for(int i = 61; i>=0; i--){
   if(ptrarray[i]!=NULL){
     free(ptrarray[i]);
   }
   }
  //  printlinkedlist();                                                                                                                                                                                          
  //printf("malloc count %d, freed count %d, realloc count %d, size of array %ld\n",mcounter,fcount,realloc,sizeof(ptrarray));
struct timeval end_time;
gettimeofday(&end_time, NULL);

return (double)(((end_time.tv_sec * 1000000) + (end_time.tv_usec)) - ((start_time.tv_sec * 1000000) + (start_time.tv_usec)));
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


return (double)(((end_time.tv_sec * 1000000) + (end_time.tv_usec)) - ((start_time.tv_sec * 1000000) + (start_time.tv_usec)));
}
*/

void printruntimes(double runtime[50][5]){

printf("Test A\t\tTest B\t\tTest C\t\tTest D\t\tTest E\n");
for (int i = 0; i<50; i++){

for (int j=0; j<5; j++){
printf("%0.f\t\t", runtime[i][j]);
}
printf("\n");	
}

}

void printmeanruntimes(double runtime[50][5]){

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

printf("\nAverage Runtimes in MICROSECONDS:\n");
printf("Test A\t\tTest B\t\tTest C\t\tTest D\t\tTest E\n");
printf("%.2f\t\t", testASum/(double)50);
printf("%.2f\t\t", testBSum/(double)50);
printf("%.2f\t\t", testCSum/(double)50);
printf("%.2f\t\t", testDSum/(double)50);
printf("%.2f\t\t", testESum/(double)50); 

printf("\n\nTotal Runtime in Seconds: %.2f\n", ((testASum+testBSum+testCSum+testDSum+testESum)/1000000) );
}


int main(int argc, char **argv){

//These are a bunch of random tests I did, I left them here so you can see	
/*	
//tests avoiding null pointer on free - this works	
float *ptra = malloc(12);
float *ptrb = malloc(42);
float *ptrc = malloc(24);
float *ptrd = malloc(50);
float *ptre = malloc(60);
printlinkedlist();
free(ptre);
printlinkedlist();
free(ptrd); 
printlinkedlist();


//tests combine method - This works
float *ptra = malloc(12);
float *ptrb = malloc(42);
float *ptrc = malloc(24);
float *ptrd = malloc(19);
float *ptre = malloc(17);
printlinkedlist();
free(ptrb);
printlinkedlist();
free(ptrc); //on this call, will combine
printlinkedlist();

//this tests the split method 
//must only split the block thats large enough if it can fit another metadata + 1 byte
//else return a slightly larger block to the user	
double *ptr1 = malloc(15);
double *ptr2 = malloc(20);
double *ptr3 = malloc(30);
free(ptr2);
malloc(3);
printlinkedlist();
	
These are all the Errors in the spec, tested tham all and they all work	
//A: Free()ing addresses that are not pointers: - THIS WORKS
	   int x;
   free( (int*)x );

//B: Free()ing pointers that were not allocated by malloc():- THIS WORKS
  char *p = (char *)malloc( 200 );	   
   free( p + 10 );
  // - or -
  int * z;
   free( z );


//C: Redundant free()ing of the same pointer: - THIS WORKS
   char  *a = (char*)malloc(100);
   free( a );
   free( a );
  // ... is an error, but:

 char *b = (char *)malloc( 100 );
   free( b );
   b = (char *)malloc( 100 );
   free( b );
//   ... is perfectly valid, even if malloc() returned the same pointer both times.

//D: Saturation of dynamic memory: - THIS WORKS
//char *c = (char*)malloc(5000);
//- or -
char *d = (char*)malloc(4080);
char *q = (char*)malloc(1);
//... your code must gracefully handle being asked for more memory than it can allocate.
*/
	



/*	
testB();
	
	
printlinkedlist();
char *ptr1 = malloc(5);
printlinkedlist();
char *ptr2 = malloc(1);
printlinkedlist();
char *ptr3 = malloc(7);
	

printlinkedlist();
free(ptr2);
printlinkedlist();
free(ptr1);
printlinkedlist();
free(ptr3);
printlinkedlist();
testA();
//testA();
  int a = 5;
  int *z=&a, *x, *y;
  x = NULL;
  y = malloc(10);
  printf("z: %p,x: %p, y: %p\n",z, x,y);
   free(x);
   //   free(z);
   free(y);


     	// int *x = malloc(4080);
  // int *y = malloc(1);

*/
// testE();
double runtime[50][5]; //initializes 50 row, 3 column array to store runtimes - will increase column size for every new test

for(int i = 0; i<50; i++){

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
	  
	 
	//column 3, Test D runtimes
	 if(j==3){
          runtime[i][j]=(testD());
	  }
	  
	  
	//column 4, Test E runtimes
        if(j==4){
          runtime[i][j]=(testE());
        }
	 
	  
	}
}
//testD();
// malloc(4080);
//malloc(63);
//printlinkedlist();
 //printf("%ld", sizeof(int));
//printruntimes(runtime);
printmeanruntimes(runtime);
//printlinkedlist(); 
return 0;
}

