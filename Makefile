#Makefile for Ising LED project


#detect the os: choose g++-4.7 for raspbpi
UNAME := $(shell uname)
ifeq ($(UNAME),Linux)
CC = g++-4.7
endif
ifeq ($(UNAME),Darwin)
CC = g++
MESSAGE = [Dont expect this to run on a mac]
endif


#Compiler: g++ -> clang, g++-4.8: gcc

#Compiler options
CFLAGS= -std=c++11 -Wall -O3
LINKFLAGS= -lncurses
HEADERS=Simulation.hpp AsciiViewer.hpp Lattice.hpp IsingPolicy.hpp LEDViewPolicy.hpp 

LEDOBJECTS=gpio.o led-matrix.o

all: isingLED tests
		@echo Compiler invocation command:  $(CC),  $(MESSAGE)

isingLED: main.o $(LEDOBJECTS)
	$(CC) $(LINKFLAGS) $(LEDOBJECTS) main.o -o isingLED

tests : tests.o $(LEDOBJECTS)
	$(CC) $(LINKFLAGS) tests.o $(LEDOBJECTS) -o tests

main.o: main.cpp $(HEADERS)
	$(CC) -c  $(CFLAGS) main.cpp

tests.o: tests.cpp $(HEADERS)
	$(CC) -c $(CFLAGS) tests.cpp
	
	
gpio.o: LED/gpio.h LED/gpio.cc
	$(CC) -c $(CFLAGS) LED/gpio.cc
	
led-matrix.o: LED/led-matrix.h LED/led-matrix.cc
	$(CC) -c $(CFLAGS) LED/led-matrix.cc
	
	


clean:
	rm -rf *.o isingLED tests


