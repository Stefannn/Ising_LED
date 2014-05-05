#Makefile for Ising LED project

#Compiler: g++ -> clang, g++-4.8: gcc
CC = g++

#Compiler options
CFLAGS=-std=c++11 -Wall -Wextra

all: isingLED tests

isingLED: main.o
	$(CC) main.o -o isingLED

tests : tests.o
	$(CC) tests.o -o tests

main.o: main.cpp Simulation.hpp
	$(CC) -c  $(CFLAGS) main.cpp

tests.o: tests.cpp Simulation.hpp
	$(CC) -c $(CFLAGS) tests.cpp

clean:
	rm -rf *.o isingLED


