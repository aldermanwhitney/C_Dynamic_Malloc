/**This file only contains mymalloc headers and definitions
 *
 */


#include <stdlib.h>
#include <stdio.h>

#ifndef __MYMALLOC_H__
#define __MYMALLOC_H__

//#include <stdlib.h>
//#include <stdio.h>

#define malloc( x ) mymalloc( x, __FILE__, __LINE__)
#define free( x ) myfree( x, __FILE__, __LINE__)

//static char myblock[4096]; //global array of 4096 bytes

struct Metadata{ 
int size; 
int isfree; 
struct Metadata *next;
};

int isInitialized();

void printbyteindex(struct Metadata* metadata);

void printuserptrindex(void* userptr);

void printlinkedlist();

int getindex(struct Metadata* metadata);

void* splitblock(struct Metadata* metadata, int bytes);

void* mymalloc(int bytes, char* file, int line);

void combineblocks(struct Metadata *block1, struct Metadata *block2);

void myfree(void *ptr, char* file, int line);

#endif
