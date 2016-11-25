imdb1: Makefile main.o bst.o readcsv.o
	gcc -g -o imdb1 main.o bst.o readcsv.o
main.o: main.c bst.h readcsv.h
	gcc -g -c main.c
bst.o: bst.c bst.h
	gcc -g -c bst.c
readcsv.o: readcsv.c readcsv.h
	gcc -g -c readcsv.c
	
imdb2: Makefile main2.o bst_ll.o readcsv.o
	gcc -g -o imdb2 main2.o bst_ll.o readcsv.o
main2.o: main2.c bst_ll.h readcsv.h
	gcc -g -c main2.c
bst_ll.o: bst_ll.c bst_ll.h
	gcc -g -c bst_ll.c