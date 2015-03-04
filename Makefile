CFLAGS += $(FLAGS)
CXXFLAGS += $(FLAGS)

CSRC = $(wildcard *.c) 
CXXSRC = $(wildcard *.cpp)
OBJ = $(CSRC:.c=.o) $(CXXSRC:.cpp=.o)


FLAGS=-O3

all: htshook refFinder


# Adjust $(HTSDIR) to point to your top-level htslib directory
HTSDIR = ../htslib
HTS = $(realpath $(HTSDIR))
HTSLIB = $(HTS)/libhts.a


.PHONY: misc clean htshook test


htshook: 
	make -C $(HTS)

-include $(OBJ:.o=.d)

%.o: %.cpp
	$(CXX) -c  $(CXXFLAGS)  -I$(HTS) $*.cpp
	$(CXX) -MM $(CXXFLAGS)  -I$(HTS) $*.cpp >$*.d


refFinder: $(OBJ)
	$(CXX) $(FLAGS)  -o refFinder *.o -lz $(HTSLIB) -lpthread




clean:
	rm -f refFinder *.o