FLAGS=-O3

all: refFinder

razf.o: razf.c razf.h
	$(CC) -c razf.c $(FLAGS)

faidx.o: faidx.c faidx.h khash.h
	$(CC) -c faidx.c $(FLAGS)

refFinder.o: refFinder.cpp razf.o faidx.o
	$(CXX) -c refFinder.cpp $(FLAGS) -o tmp.o
	ld -r tmp.o faidx.o razf.o -o refFinder.o 

refFinder: refFinder.o refFinder_main.cpp 
	$(CXX) refFinder_main.cpp $(FLAGS) refFinder.o -o refFinder -lz




clean:
	rm -f refFinder *.o