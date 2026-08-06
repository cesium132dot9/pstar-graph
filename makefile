CFLAGS = -g -Wall -Wextra -std=c++20

.PHONY : all main test clean

all : main test

# the linker appends .exe on this platform, so the real targets carry it
main : main.exe
test : test.exe

main.exe : main.o pstar.o graph.o trie.o
	g++ ${CFLAGS} -o main.exe main.o pstar.o graph.o trie.o

test.exe : test.o pstar.o graph.o trie.o
	g++ ${CFLAGS} -o test.exe test.o pstar.o graph.o trie.o

main.o : main.cpp pstar.h graph.h trie.h
	g++ ${CFLAGS} -c main.cpp

test.o : test.cpp pstar.h graph.h trie.h
	g++ ${CFLAGS} -c test.cpp

pstar.o : pstar.cpp pstar.h graph.h
	g++ ${CFLAGS} -c pstar.cpp

graph.o : graph.cpp graph.h
	g++ ${CFLAGS} -c graph.cpp

trie.o : trie.cpp trie.h pstar.h graph.h
	g++ ${CFLAGS} -c trie.cpp

clean :
	rm -f *.o main main.exe test test.exe
