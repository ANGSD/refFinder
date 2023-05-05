FLAGS=-O3 -fPIC

CFLAGS += $(FLAGS)
CXXFLAGS += $(FLAGS)

CSRC = $(wildcard *.c) 
CXXSRC = $(wildcard *.cpp)
OBJ = $(CSRC:.c=.o) $(CXXSRC:.cpp=.o)

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
