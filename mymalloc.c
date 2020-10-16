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

static char myblock[4096]; //global variable for memory
void break_up(){

}

void combine(){

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
printf("traversing with int pointer\n");


printf("intptr points to: %d", *firstintptr);
firstintptr++;

intcount++;

}

printf("\nints traversed: %d\n", intcount);



//printf("int: %d", ptr);
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
  return 0;
}
