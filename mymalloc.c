#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
//#define malloc( x ) mymalloc( x, __FILE__, __LINE__ ) //uncomment these two lines, and main to only test mymalloc.c without memgrind
//#define free( x ) myfree( x, __FILE__, __LINE__ )

/**This file is for our mymalloc and myfree function implementations
 *There will not be a main in this file, will need to link when compiling
Compile this with gcc mymalloc.c -c to make it an object file instead of an executeable
or, with gcc -Wall -Werror mymalloc.c -c
*/

static char myblock[4096]; //global array of 4096 bytes

//This definition is now in the header file
/*
struct Metadata{ //size of our metadata is 16 bytes
int size; //number of bytes to be malloced
int isfree; //1 for true, 0 for false
struct Metadata *next;
};
*/
/**Returns true (1) if the memory array is NOT all zeros
 *This means that it has been initialized and contains metadata
 */
int isInitialized(){

int *ptr = (int*)&myblock[0];

while(ptr<(int*)&myblock[4096]){
//printf("ptr: %d", *ptr);
if (*ptr!=0){
return 1;
}
ptr++;
}

return 0;	
}

/**These are just print functions to make our lives easier
 *The print linked list one is probably the most useful
 */

//Given a pointer to a meta data block, will return how many bytes from beginnning of array to metadata
void printbyteindex(struct Metadata* metadata){
printf("metadata at myblock[%ld:%ld]\n", ((char *)metadata - (char *)&myblock[0]), ((char*) metadata - (char *)&myblock[0])+16);	
}

//Given a void or char pointer, will return the index of the array where the users pointer points
void printuserptrindex(void* userptr){
printf("userptr at myblock[%ld]\n", ((char *)userptr - (char *)&myblock[0]));	
}

//This will give a full print out of the entire linked list
void printlinkedlist(){//struct Metadata* firstmetadata){

struct Metadata *firstmetadata = (void*)&myblock[0]; //set a struct metadata pointer pointing for first spot in myblock
if(!(isInitialized())){
puts("the memory is uninitialized\n");
return;
}

struct Metadata *current = firstmetadata;

printf("\n");
printf("META DATA LINKED LIST:\n");
while (current!=NULL){
printbyteindex(current);
printf("[ size: %d  ", current->size);
printf("isfree: %d ]\n", current->isfree);
printf("\t|\n");
printf("\tV\n");
current = current->next;	
}
printf("\tNULL\n\n");
}

//Given a pointer to a metadata struct, will return the index in the array that it STARTS at
//note: struct will end at returned value + 16
int getindex(struct Metadata* metadata){
return ((char * )metadata - (char * )&myblock[0]);
} 






/**Parameters: metadata containing info of block to be split, and number of bytes user needs
 *will return a void pointer to the byte immediately following the metadata
 */
void* splitblock(struct Metadata* metadata, int bytes){
	
int blockexcess = (metadata->size)-bytes;
void* userptr = (void*)metadata + sizeof(struct Metadata) + 1; //pointer to first spot after meta data

// The block contains more bytes than the user requested
// either create a new block with remaining bytes
// or if too small, give user the byte after the current meta data block  (will have a few extra bytes, this is fine)

//create new block
if (blockexcess>sizeof(struct Metadata)){
puts("Block is large enough to split. creating new block\n");
struct Metadata *newmetadata = (void*)&myblock[getindex(metadata)+sizeof(struct Metadata)+bytes]; 
newmetadata->isfree = 1;
newmetadata->size = blockexcess-sizeof(struct Metadata);
newmetadata->next = metadata->next;

metadata->next = newmetadata;
metadata->size = bytes;
metadata->isfree = 0;
	
}
else //case where the block is two small to split, return a slightly larger size to the user
{
//printf("could not split block, not large enough");	
metadata->isfree=0;
}


//printf("userdata points to myblock[%ld]\n", ((char *)userptr - (char *)&myblock[0]));	
return userptr;
}


void* mymalloc(int bytes, char* file, int line){

struct Metadata *firstmetadata = (void*)&myblock[0]; //set a struct metadata pointer pointing for first spot in myblock

printf("\nmalloc(%d):\n", bytes);
//printf("Has the memory already been initialized?: %d\n", isInitialized());

//if memory has not been accessed/malloced before
if (!(isInitialized())){

//If this is the first call to malloc, create first metadata block	
firstmetadata->isfree=1;
firstmetadata->size = 4096-sizeof(struct Metadata);
firstmetadata->next=NULL;
}

struct Metadata *current = firstmetadata;

//iterate through metadata linked list
//searching for the first metadata block, if any
//that is large enough and free
while(current!=NULL){
if (((current->size)>=bytes)&& (current->isfree==1)){
printf("found block which matches malloc request\n");
void* userptr = splitblock(current, bytes);
//printlinkedlist(firstmetadata);
return userptr;
}		
current = current->next;
}

//reached the end of the linked list - no block found which is big enough and free
printf("ERROR: (File:%s Line:%d) Memory is not large enough for request\n", file, line);	
return NULL;
}


/**Given pointers to two free metadata blocks (must be given in sequential order)
 *Will combine into a single, free metadata block
 */
void combineblocks(struct Metadata *block1, struct Metadata *block2){

//if either blocks are null, do not combine	
if ((block1==NULL)||(block2==NULL)){
return;	
}
	
//do not combine if both blocks are not free	
if ((block1->isfree==0) || (block2->isfree==0)){
return;
}
 if(block2->next!=NULL){
   block1->next = block2->next;
 }
 else{
   block1->next = NULL;
 }
block1->size=((block1->size) + (block2->size) + sizeof(struct Metadata));
block2=NULL;

printf("adjacent free blocks found and combined.\n");
return; 
}


void myfree(void *ptr, char* file, int line){

  if(ptr==NULL){
    printf("ERROR: (File:%s Line:%d) Pointer is NULL, not allocated in memory or already freed\n", file, line);
    return;
  }

//no memory has been allocated, nothing to free
if(!(isInitialized())){
printf("ERROR: (File:%s Line:%d) No pointer found to free\n", file, line);	
return;
}
 
struct Metadata *firstmetadata = (void*)&myblock[0]; //set a struct metadata pointer pointing for first spot in myblock

 struct Metadata *next;
struct Metadata *current = firstmetadata;
struct Metadata *prev = NULL;
 if(current->next!=NULL){
   next = current->next;
 }
//iterate through metadata linked list
//look at all nonfree metadata
//check each byte after the metadata
//searching for a pointer which matches the parameter pointer

while(current!=NULL){
if (current->isfree==0){

//check byte after metadata	
void* userptr = (void*)current + sizeof(struct Metadata) + 1; //pointer to first spot after meta data

//if this matches the given pointer, found a match. remove and return
if(userptr==ptr){
current->isfree=1;	
userptr = NULL; //throw away user pointer
printf("found the user pointer, and freed it\n");

//combine with previous or next blocks if applicable
//combineblocks(prev, current);
combineblocks(current, next);
combineblocks(prev, current); 
//printlinkedlist();
return;
}
}
 if(current!=NULL){
   prev = current;
 }
 if(current->next!=NULL){
current = current->next;
 }
 if(next->next!=NULL){
next = next->next;
}
 //prev = current;
//current = current->next;
}


printf("ERROR: (File:%s Line:%d) No pointer found to free\n", file, line);	
//printf("No such pointer found to free\n");
return;
}

/*
//This can be commented out for purposes of using make memgrind. If you want to test here, just compile mymalloc.c regularly
int main(){


//Just some preliminary tests for mymalloc, can change these around and use any pointer type		
char* call1 = malloc(5);
char* call2 = malloc(8);
char* call3 = malloc(7);

printlinkedlist(); //check output with this

printuserptrindex(call1); //this just tests to see that the byte returned to the user is 1 byte after the (correct) metadata
printuserptrindex(call2);
printuserptrindex(call3);

//test some free calls
free(call3);

int z = 5;
int* x = &z;

free(x);
//printuserptrindex(call3); //this just tests to see that the byte returned to the user is 1 byte after the (correct) metadata

//for(int i=0; i<240; i++){	
//malloc(1);
//}
 // printmemory();
  return 0;
}
*/
