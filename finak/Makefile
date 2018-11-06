CXX = g++

CXXFLAGS = -g -Wall -std=c++0x

COMPILE = $(CXX) $(CXXFLAGS) -c

EXE = lisp

OBJS = front-end.o Examiner.o S_Exprsn.o 

$(EXE) : $(OBJS)
	$(CXX) $^ -o $@

clean : 
	rm *.o lispintrepreter

%.o : %.cc
	$(COMPILE) -o $@ $<

front-end.o : Examiner.h

Examiner.o : Examiner.h

S_Exprsn.o : S_Exprsn.h
