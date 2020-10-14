#include <stdio.h>
#include <stdlib.h>

/**This file is for our mymalloc and myfree function implementations
 *There will not be a main in this file, will need to link when compiling
Compile this with gcc mymalloc.c -c to make it an object file instead of an executeable
or, with gcc -Wall -Werror mymalloc.c -c
*/

static char myblock[4096]; //global variable for memory

void mymalloc(){
printf("myblock[4096] %c", myblock[0]); //this is just here temporarily for compilation

}

void myfree(){

}
