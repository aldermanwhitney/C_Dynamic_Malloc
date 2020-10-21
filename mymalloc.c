#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

/**This file is for our mymalloc and myfree function implementations
*/


static char myblock[4096]; //global array of 4096 bytes


/**Returns true (1) if the memory array is NOT all zeros
 *This means that it has already been initialized and contains metadata
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


/**These are just print functions for debugging purposes
 */

//Given a pointer to a meta data block, will return how many bytes from beginnning of array to metadata
void printbyteindex(struct Metadata* metadata){
printf("metadata at myblock[%ld:%ld]\n", ((char *)metadata - (char *)&myblock[0]), ((char*) metadata - (char *)&myblock[0])+15);	
}

//Given a pointer, will return the index of the array where the users pointer points
void printuserptrindex(void* userptr){
printf("userptr at myblock[%ld]\n", ((char *)userptr - (char *)&myblock[0]));	
}

//This prints the entire linked list
void printlinkedlist(){

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
//note: struct will end at returned value + 15  (ie struct starts at 0 and ends at 15)
int getindex(struct Metadata* metadata){
return ((char * )metadata - (char * )&myblock[0]);
} 






/**Parameters: pointer to metadata containing info of block to be split, and number of bytes user needs
 *will return a void pointer to the byte immediately following the metadata
 */
void* splitblock(struct Metadata* metadata, int bytes){
	
int blockexcess = (metadata->size)-bytes;
void* userptr = (void*)metadata + sizeof(struct Metadata); //pointer to first spot after meta data

// The block contains more bytes than the user requested
// either create a new block with remaining bytes
// or if too small, give user the byte after the current meta data block  (will have a few extra bytes, this is fine)

//create new block if the excess size is large enough to store a metadata and at least 1 byte
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
 //case where the block is two small to split
 //but large enough for user data, 
 //return a slightly larger size to the user
else
{
printf("could not split block, not large enough");	
metadata->isfree=0;
}

return userptr;
}

/**Function takes the amount of bytes the user is requesting as an arguement
 *On success, returns void pointer to first byte of data allocated to user after metadata
 On failure, returns null and reports file and line number which cause the error
 */
void* mymalloc(int bytes, char* file, int line){

if (bytes==0){
printf("ERROR: (File:%s Line:%d) Cannot return a pointer to a 0 byte block\n", file, line);	
return NULL;
}

//set a struct metadata pointer pointing to the first spot in myblock
struct Metadata *firstmetadata = (void*)&myblock[0]; 

printf("\nmalloc(%d):\n", bytes);

//if memory has not been accessed/malloced before
//create first metadata block
if (!(isInitialized())){
	
firstmetadata->isfree=1;
firstmetadata->size = 4096-sizeof(struct Metadata);
firstmetadata->next=NULL;
}

struct Metadata *current = firstmetadata;

//iterate through metadata linked list
//searching for the first metadata block, if any
//that is both large enough and free
while(current!=NULL){
if (((current->size)>=bytes)&& (current->isfree==1)){
printf("found block which matches malloc request\n");
void* userptr = splitblock(current, bytes);
//printlinkedlist(firstmetadata);
printf("MALLOC RETURN POINTER: ");
printuserptrindex(userptr);
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

//do not combine if block1 and block2 point to same block
if(block1==block2){
return;
}

//else, combine blocks
block1->next = block2->next;
block1->size=((block1->size) + (block2->size) + sizeof(struct Metadata));
block2=NULL;

printf("adjacent free blocks found and combined.\n");
return; 
}

/**Function takes a pointer of any type 
 *On success, allows the data at that block available to be allocated again
On failure, prints an error message containing the file and line of the bad call
 */
void myfree(void *ptr, char* file, int line){


if(!(isInitialized())){
printf("ERROR: (File:%s Line:%d) No such pointer found to free\n", file, line);
return;
}


if(ptr==NULL){
    printf("ERROR: (File:%s Line:%d) Cannot Free a NULL pointer\n", file, line);
    return;
}

//set a struct metadata pointer pointing to first spot in myblock
struct Metadata *firstmetadata = (void*)&myblock[0]; 

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

//pointer to first byte after metadata	
void* userptr = (void*)current + sizeof(struct Metadata);
//printf("FREE PTR ITERATION:  ");
//printuserptrindex(userptr);

//if this matches the given pointer, found a match
//free block
if(userptr==ptr){
current->isfree=1;	

printuserptrindex(userptr);
printf("found the user pointer, and freed it\n");

userptr = NULL; //throw away user pointer

//combine with previous or next blocks if applicable
combineblocks(prev, current);
combineblocks(current, next); 
//printlinkedlist();
return;
}
}

prev = current;
current = current->next;

//Avoids null pointer, at some point next will equal null
//This is fine
//combine method will catch this
//The only reason we need a next pointer is for the combine method
if(next==NULL){
continue;
}
next = next->next;

}


printf("ERROR: (File:%s Line:%d) No such pointer found to free\n", file, line);	
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
