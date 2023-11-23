
CFLAGS =-Wall -g -std=c++11 -fsanitize=address 

all: HashTable.o HashTableIterator.o ExtHashTable.o libHashTable main clean

HashTable.o: HashTable.cpp
	g++ ${CFLAGS} HashTable.cpp -c

HashTableIterator.o: HashTableIterator.cpp
	g++ ${CFLAGS} HashTableIterator.cpp -c 

ExtHashTable.o: ExtHashTable.cpp
	g++ ${CFLAGS} ExtHashTable.cpp -c

libHashTable: HashTable.o HashTableIterator.o ExtHashTable.o
	ar rcs libHashTable.a HashTable.o HashTableIterator.o ExtHashTable.o

main: libHashTable main.cpp
	g++ ${CFLAGS} t19.cpp -o main -lHashTable -L.

clean:
	rm *.o