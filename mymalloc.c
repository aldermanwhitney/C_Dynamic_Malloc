#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#define malloc( x ) mymalloc( x, __FILE__, __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )
/**This file is for our mymalloc and myfree function implementations
 *There will not be a main in this file, will need to link when compiling
Compile this with gcc mymalloc.c -c to make it an object file instead of an executeable
or, with gcc -Wall -Werror mymalloc.c -c
*/

static char myblock[4096] = {4,'z','y','x','w', 2,'x','d',3}; //global variable for memory
void break_up(){

}

void combine(){

}

void printmemory(){
int * p = (int*)(&myblock[0]);
int toAdd = 0;

while ((void *)p < (void *)&myblock[4096-1] && *p!=0){		
int oldValue = *p;
printf("found a %d at %li\n", *p, ((void *)p - (void *)&myblock[0]));
p = (int *) (&myblock[(toAdd+(oldValue&~1) + 4)]);
toAdd+=(oldValue&~1)+4;														}
}

void mymalloc(int bytes, char* file, int line){

char *ptr = &myblock[0];
int count = 0;


while(ptr!=&myblock[4097]){
//printf("traversing with char pointer\n");	
count++;
ptr++;
}

printf("\nbytes traversed: %d\n", count);



int *firstintptr = (int *)&myblock[0];
int *lastintptr = (int *)&myblock[4096];
int intcount = 0;

while(firstintptr!=lastintptr){
//printf("traversing with int pointer\n");
//printf("intptr points to: %d", *firstintptr);
firstintptr++;
intcount++;
}

printf("\nints traversed: %d\n", intcount);



int *firstmetadata = (int *)&myblock[0];
int *lastmetadata = (int *)&myblock[4096];
//(int *)myblock[0]= 7;
//(int *)myblock[4096]= 1;

*firstmetadata = 4;
*lastmetadata = 9;

int count5 = 0;

while(firstmetadata!=lastmetadata){
printf("intptr points to: %d\n", *firstmetadata);
firstmetadata++;
count5++;
}

printf("intptr points to: %d\n", *firstmetadata);
printf("traversed %d\n", count5);





int *metadata = (int *)&myblock[0]; //pointer to first spot in array where meta data is stored
void *current = (void *)&myblock[0]; //pointer to very first byte in array
//void *prev = NULL;

int count7 = 0;
int metadata_size = sizeof(int);
printf("metadata size: %d\n", metadata_size);
printf("size of block %ld\n", sizeof(myblock));
printf("size of block[0] %ld\n", sizeof((myblock[0])));
while((void *)current < (void *)&myblock[4096]){ //iterate through memory array byte by byte

//we have found a non free memory, move current that number of bytes forward + size of metadata
if (*metadata!=0){
puts("\n");	
printf("found metadata value %d at %li\n", *metadata, ((int *)metadata - (int *)&myblock[0]));	

//pointer pointing to address that metadata pointer 
printf("found metadata value %d at %ls\n", *metadata, metadata);
printf("metadata not zero, metadata is %d\n", *metadata);	

printf("metadata address: %p\n", metadata);
printf("current address: %p\n", current);

printf("metadata dec address%lu\n", (unsigned long)metadata);
printf("current dec address%lu\n", (unsigned long)current);

current = (void *)&myblock[(*metadata) + metadata_size]; //move current pointer past user bytes of data
//current = (void *)&myblock[5];

printf("current moved to address: %p\n", current);
//printf("current moved to: %c\n", *((char *)current));

printf("metadata value + metadata size: %d\n", (((int)(*metadata))+metadata_size));

metadata = (int *)current;

printf("metadata points to: %d\n", *metadata);
printf("metadata address: %p\n", metadata);



printf("metadata dec address%lu\n", (unsigned long)metadata);
printf("current dec address%lu\n", (unsigned long)current);
count7++;
}
else{
current++;
count7++;
}
//break;
//printf("current: %c", *current);
}

printf("count7: %d\n", count7);


/*

int *currentmetadata = (int *)&myblock[0]; //pointer to first spot in array where meta data is storeda, will traverse list with this
//int *prevmetadata = NULL:
int *lastmetadata2 = (int *)&myblock[4096];
//void *current = (void *)&myblock[0]; //pointer to very last byte in array
while(currentmetadata < lastmetadata2){ //iterate through memory array byte by byte

int currentvalue = (int)*currentmetadata;	
//currentmetadata = currentmetadata + currentvalue;	

currentmetadata+=currentvalue;

}

*/
}

 
  /*
  int value;
  int* current;
  myblock[0]= (int) value;
  current = &value;
  //current = &myblock[0];
  while(value!=0){

    current = current+value+sizeof(int);
    value = *current;
  }

  if(value == 0) {

  }

  int md_current, md_prev, md_next; //three ints to track metadata i.e, size of current, previous and next block 
  int *current,*prev, *next; //three pointers for currennt, previous and next block

   first malloc call
     we set the metadata of first block as 0 to indicate free space
     we set the current pointer to the space right after current metadata
  
  if(md_prev==0 && md_next==0){
    md_current = 0;
    myblock[0]=md_current;
    current =(int*) &myblock[0]+4;
    md_prev=-1;
    md_next=0;
  }

  if the current block is full, we malloc to next block 
    and move current pointer to that block
  
  if(md_current!=0 && current!=NULL && md_next==0 && bytes>0){
    prev = current;
    md_prev=md_current;
    current = current+md_current+4;
    md_current = bytes;
    next=current+md_current+4;//records a pointer to next free space
    md_next=0;
  }
  
    very first block is empty
    so add data there
  
  if(bytes>0 && md_current==0 && current!=NULL){
    md_current=bytes;
    next=current+md_current+4;
    md_next=0;
    md_prev=-1;
  }

  } 
*/
  //printf("myblock[4096] %d", myblock[0]); //this is just here temporarily for compilation



void myfree(){

}

int main(){
  malloc(5);
  printmemory();
  return 0;
}
