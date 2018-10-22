CXX = g++
CXXFLAGS = -g  -std=c++11
#CXXFLAGS = -g -Wall -std=c++11
COMPILE = $(CXX) $(CXXFLAGS) -c
EXE = front-end
OBJS = front-end.o

$(EXE): $(OBJS)
	$(CXX) $^ -o $@

front-end.o: front-end.cpp

clean : 
	rm *.o front-end

