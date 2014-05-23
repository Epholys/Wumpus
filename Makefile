# Makefile for HuntTheWumpus
#
# Compiler and Linker: g++ 4.8.2
#
# See LICENSE.txt for the license
#

DIRS = src

all:
	for d in $(DIRS); do $(MAKE) -C $$d; done
	\mv src/HuntTheWumpus.bin ./

clean:
	for d in $(DIRS); do $(MAKE) -C $$d clean; done

