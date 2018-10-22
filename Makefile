CXX = g++
CXXFLAGS = -g  -std=c++11
COMPILE = $(CXX) $(CXXFLAGS) -c
EXE = P1
OBJS = P1.o

$(EXE): $(OBJS)
	$(CXX) $^ -o $@
P1.o: P1.cpp
clean : 
	rm *.o P1