FLAGS=-O3

CFLAGS += $(FLAGS)
CXXFLAGS += $(FLAGS)

CSRC = $(wildcard *.c) 
CXXSRC = $(wildcard *.cpp)
OBJ = $(CSRC:.c=.o) $(CXXSRC:.cpp=.o)


-include $(OBJ:.o=.d)


all: refFinder

%.o: %.cpp
	$(CXX) -c  $(CXXFLAGS)  $*.cpp
	$(CXX) -MM $(CXXFLAGS)  $*.cpp >$*.d


%.o: %.c
	$(CC) -c  $(CFLAGS) -I$(HTS) $*.c
	$(CC) -MM $(CFLAGS)  -I$(HTS) $*.c >$*.d


refFinder: $(OBJ)
	$(CXX) $(FLAGS)  -o refFinder *.o -lz -lpthread




clean:
	rm -f refFinder *.o