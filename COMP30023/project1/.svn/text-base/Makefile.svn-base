# 
# Makefile for project 1 (COMP30023)
#
# by Shi Chang Zhang, shiz2, 695434
#
# Description: Project 1 consists of two scheduling algorithms, 
# first come first served and multi-level feedback queue, and an
# algorithm for allocating memory to a process, the first fit algorithm.
#


## CC  = Compiler.
## CFLAGS = Compiler flags.
CC	= gcc
CFLAGS 	= -Wall -g


## OBJ = Object files.
## SRC = Source files.
## EXE = Executable name.

SRC =		main.c reader.c fcfs.c memory.c multi.c
OBJ =		main.o reader.o fcfs.o memory.o multi.o
EXE = 		simulation

## Top level target is executable.
$(EXE):	$(OBJ)
		$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -lm


## Clean: Remove object files and core dump files.
clean:
		/bin/rm $(OBJ) 

## Clobber: Performs Clean and removes executable file.

clobber: clean
		/bin/rm $(EXE) 

## Dependencies

main.o: main.c process.h queue.h memory.h Makefile
reader.o: reader.c reader.h process.h Makefile
fcfs.o: fcfs.c queue.h process.h memory.h
memory.o: memory.c memory.h
multi.o: multi.c queue.h process.h memory.h