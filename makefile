CC = g++ 
CFLAGS = -g -Wall -std=c++11 

project: p5130.o
	g++ p5130.o -o project
 
p5130.o: p5130.cpp
	g++ -c p5130.cpp

clean:
	rm p5130.o project
