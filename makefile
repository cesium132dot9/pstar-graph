CFLAGS = -g -Wall -Wextra

main : main.o pstar.o graph.o
	g++ ${CFLAGS} -o main main.o pstar.o graph.o

main.o : main.cpp pstar.h graph.h
	g++ ${CFLAGS} -c main.cpp 

pstar.o : pstar.cpp graph.h
	g++ ${CFLAGS} -c pstar.cpp

graph.o : graph.cpp graph.h 
	g++ ${CFLAGS} -c graph.cpp

clean : 
	rm *.o main