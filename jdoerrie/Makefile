CC=g++
CXXFLAGS=-std=c++11 -lboost_regex
OPTFLAGS=-O3
DBGFLAGS=-O0 -g

all:
	$(CC) $(CXXFLAGS) $(OPTFLAGS) *.cpp -o Evaluator

opt:
	$(CC) $(CXXFLAGS) $(OPTFLAGS) *.cpp -o Evaluator

dbg:
	$(CC) $(CXXFLAGS) $(DBGFLAGS) *.cpp -o Evaluator

clean:
	rm Evaluator
