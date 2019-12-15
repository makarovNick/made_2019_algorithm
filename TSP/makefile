CC = g++
FLAGS = 

all : run

run : main.o DSU.o graph.o
	$(CC) $(FLAGS) -o result main.o graph.o DSU.o

DSU.o : DSU.cpp
	$(CC) -c DSU.cpp

main.o : main.cpp
	$(CC) -c main.cpp

graph.o : graph.cpp
	$(CC) -c graph.cpp

clean : 
	rm -f *.o result