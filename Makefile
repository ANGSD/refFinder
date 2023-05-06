FLAGS=-O3 -fPIC

CFLAGS += $(FLAGS)
CXXFLAGS += $(FLAGS)

CSRC = $(wildcard *.c) 
CXXSRC = $(wildcard *.cpp)
OBJ = $(CSRC:.c=.o) $(CXXSRC:.cpp=.o)

.PHONY: all clean test

all: refFinder reffinder.so reffinder.a

-include $(OBJ:.o=.d)

%.o: %.cpp
	$(CXX) -c  $(CXXFLAGS)  $*.cpp
	$(CXX) -MM $(CXXFLAGS)  $*.cpp >$*.d

%.o: %.c
	$(CC) -c  $(CFLAGS) $*.c
	$(CC) -MM $(CFLAGS) $*.c >$*.d

refFinder: $(OBJ)
	$(CXX) $(FLAGS)  -o refFinder *.o -lz -lpthread

reffinder.so: $(OBJ)
	$(CXX) -shared $(FLAGS)  -o libreffinder.so bgzf.o faidx.o hfile.o hfile_net.o knetfile.o refFinder.o -lz

reffinder.a: $(OBJ)
	ar -rcs libreffinder.a bgzf.o faidx.o hfile.o hfile_net.o knetfile.o refFinder.o

clean:
	rm -f refFinder *.o *.so *.a

test: test/MT.fa.gz test/MT.fa.gz.fai test/input.tsv
	echo "test1) will print out a single site"
	echo "MT 200" | ./refFinder test/MT.fa.gz
	echo "test2) will print out a region" 
	echo "MT:200-210" | ./refFinder test/MT.fa.gz
	echo "test3) will print out a region" 	
	cat test/input.tsv | ./refFinder test/MT.fa.gz
