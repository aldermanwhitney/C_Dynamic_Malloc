memgrind: memgrind.o mymalloc.o
		gcc -c -Wall memgrind.o mymalloc.o -o memgrind

memgrind.o: memgrind.c mymalloc.h
		gcc -c -Wall memgrind.c

mymalloc.o: mymalloc.c mymalloc.h
		gcc -c -Wall mymalloc.c

clean:
		rm -f memgrind *.o
