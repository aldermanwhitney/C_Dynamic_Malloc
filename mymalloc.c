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

static char myblock[4096];

struct Metadata{
int size;
int isfree;
struct Metadata *next;
};
//struct Metadata* firstmetadata = NULL;



void printbyteindex(struct Metadata* metadata){


printf("metadata at myblock[%ld:%ld]\n", ((char *)metadata - (char *)&myblock[0]), ((char*) metadata - (char *)&myblock[0])+16);	
}


void printlinkedlist(struct Metadata* firstmetadata){
printf("\n");
struct Metadata *current = firstmetadata;
	
while (current!=NULL){
printbyteindex(current);
printf("[size: %d ", current->size);
printf("isfree: %d ]\n", current->isfree);

current = current->next;	
}
}

int getindex(struct Metadata* metadata){
return ((char * )metadata - (char * )&myblock[0]);
} 


/**Parameters: metadata containing info of block to be split, and number of bytes user needs
 *
 */
void splitblock(struct Metadata* metadata, int bytes){

int oldsize = metadata->size;	
int blockexcess = (metadata->size)-bytes;

// The block contains more bytes than the user requested
// either create a new block with remaining bytes
// or if too small, give the user a few extra bytes

//create new block
if (blockexcess>sizeof(struct Metadata)){
void* userptr = (void*)metadata + sizeof(struct Metadata) + 1; //pointer to first spot after meta data
struct Metadata *newmetadata = (void*)&myblock[getindex(metadata)+sizeof(struct Metadata)+bytes]; 
newmetadata->isfree = 1;
newmetadata->size = blockexcess-sizeof(struct Metadata);
newmetadata->next = metadata->next;

metadata->next = newmetadata;
metadata->size = bytes;
metadata->isfree = 0;
//printf("metadata points to byte %ld\n", ((char *)metadata - (char *)&myblock[0]));	
}


}

void combineblocks(){

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

/**Returns false (0) if the memory array is all zeros
 *This means that it has not yet been malloced
 */
int isfirstmalloc(){

int *ptr = (int*)&myblock[0];

while(ptr<(int*)&myblock[4096]){
if (*ptr!=0){
return 0;
}
ptr++;
}

return 1;	
}


void mymalloc(int bytes, char* file, int line){

struct Metadata *firstmetadata = (void*)&myblock[0]; //set a struct metadata pointer pointing for first spot in myblock

printf("\n\nmalloc(%d)\n", bytes);
printf("is this the first malloc?: %d\n", isfirstmalloc());
if (isfirstmalloc()){

//If this is the first call to malloc, create first metadata block	
firstmetadata->isfree=1;
firstmetadata->size = 4096-sizeof(struct Metadata);
firstmetadata->next=NULL;

//dont need this yet
void* userptr = (void*)firstmetadata + sizeof(struct Metadata) + 1;

printf("(int)firstmetadata points at myblock[%ld]\n", ((int *)firstmetadata - (int *)&myblock[0]));	
printf("(int) userptr points at myblock[%ld]\n", ((int *)userptr - (int *)&myblock[0]));	
printf("(char)firstmetadata points at myblock[%ld]\n", ((char *)firstmetadata - (char *)&myblock[0]));	
printf("(char) userptr points at myblock[%ld]\n", ((char *)userptr - (char *)&myblock[0]));	

printf("firstmetadata points at byte %ld\n", ((char *)firstmetadata - (char *)&myblock[0]));	
printf("userptr points at byte %ld\n", ((char *)userptr - (char *)&myblock[0]));	
//return;
}

struct Metadata *current = firstmetadata;
struct Metadata *prev = NULL;
//while((char*)current<=&myblock[4096]){
while(current!=NULL){

if ((current->size)>=bytes){
printf("found block which matches request. Splitting..\n");
splitblock(current, bytes);
printlinkedlist(firstmetadata);
return;
}		
prev = current;
current = current->next;
}
//reached the end of the linked list - no block found which is big enough
if (current==NULL){
//return NULL
}	


}


void myfree(){

}

int main(){
 
malloc(5);
malloc(8);
malloc(7);	
//for(int i=0; i<3; i++){	
//malloc(1);
//}
 // printmemory();
  return 0;
}
