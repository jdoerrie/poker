CC=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -Werror -pedantic
LIBS=-lboost_regex
OPTFLAGS=-O2
DBGFLAGS=-O0 -g

all:
	$(CC) $(CXXFLAGS) $(OPTFLAGS) *.cpp -o Evaluator $(LIBS)

opt:
	$(CC) $(CXXFLAGS) $(OPTFLAGS) *.cpp -o Evaluator $(LIBS)

dbg:
	$(CC) $(CXXFLAGS) $(DBGFLAGS) *.cpp -o Evaluator $(LIBS)

clean:
	rm Evaluator
