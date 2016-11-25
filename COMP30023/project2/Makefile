# Makefile for project 2
# COMP30023 Computer Systems
# Semester 1, 2016
#
# by Shi Chang Zhang, 695434, shiz2
#	
# Description: Simple server and client for the game
# 	       Mastermind.

CC=gcc
CFLAGS=-Wall -lpthread

# General files like header files and Makefile

HEAD=mastermind.h Makefile

# Server sources and objects

SRCS=server.c mastermind.c queue.c
OBJS=$(SRCS:.c=.o)

# Client sources and objects

SRCC=client.c
OBJC=$(SRCC:.c=.o)

## Executables

EXE=server client

all: $(EXE)

server: $(OBJS) 
	$(CC) $(CFLAGS) -o $@ $(OBJS) 
client: $(OBJC)
	$(CC) $(CFLAGS) -o $@ $(OBJC) 

## Clean - clean dem files

clean:
	/bin/rm *.o $(EXE) log.txt

## Dependencies

server.o: server.c queue.h $(HEAD)
client.o: client.c $(HEAD)
mastermind.o: mastermind.c $(HEAD)
queue.o: queue.c queue.h
