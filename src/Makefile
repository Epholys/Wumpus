# Makefile for HuntTheWumpus
#
# Compiler and Linker: g++ 4.8.2
# 
# See LICENSE.txt for the license
#

OBJS=Action_all.o Assemblage.o Entity_manager.o \
     Input_player.o main.o menu.o System_all.o \
     System_player.o System_helper.o
CC=g++
CFLAGS=-std=c++11 -O2 -c
LDFLAGS=
EXECUTABLE=HuntTheWumpus.bin

all: $(OBJS)
	$(CC) $(LDFLAGS) -o $(EXECUTABLE) $(OBJS)

Action_all.o:
	$(CC) $(CFLAGS) Action_all.cpp

Assemblage.o:
	$(CC) $(CFLAGS) Assemblage.cpp

Entity_manager.o:
	$(CC) $(CFLAGS) Entity_manager.cpp
 
Input_player.o:
	$(CC) $(CFLAGS) Input_player.cpp

main.o:
	$(CC) $(CFLAGS) main.cpp

menu.o:
	$(CC) $(CFLAGS) menu.cpp

System_all.o:
	$(CC) $(CFLAGS) System_all.cpp

System_player.o:
	$(CC) $(CFLAGS) System_player.cpp

System_helper.o:
	$(CC) $(CFLAGS) System_helper.cpp


clean:
	\rm *.o
