#Makefile for Ising LED project

#Compiler: g++ -> clang, g++-4.8: gcc
CC = g++

#Compiler options
CFLAGS= -std=c++11 -Wall -Wextra
LINKFLAGS= -lncurses
HEADERS=Simulation.hpp AsciiViewer.hpp Lattice.hpp IsingPolicy.hpp


all: isingLED tests

isingLED: main.o
	$(CC) $(LINKFLAGS) main.o -o isingLED

tests : tests.o
	$(CC) $(LINKFLAGS) tests.o -o tests

main.o: main.cpp $(HEADERS)
	$(CC) -c  $(CFLAGS) main.cpp

tests.o: tests.cpp $(HEADERS)
	$(CC) -c $(CFLAGS) tests.cpp
	


clean:
	rm -rf *.o isingLED testing


