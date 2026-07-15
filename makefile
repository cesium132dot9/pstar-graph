CFLAGS = -g -Wall -Wextra

pstar : pstar.o graph.o 
	g++ ${CFLAGS} -o pstar pstar.o graph.o

pstar.o : pstar.cpp graph.o
	g++ ${CFLAGS} -c pstar.cpp

graph.o : graph.h 
	g++ ${CFLAGS} -c graph.cpp

clean : 
	rm *.o pstar