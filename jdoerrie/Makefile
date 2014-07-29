CC=g++
OPTFLAGS=-O3 -std=c++11
DBGFLAGS=-O0 -g -std=c++11

all:
	$(CC) $(OPTFLAGS) *.cpp -o Evaluator

opt:
	$(CC) $(OPTFLAGS) *.cpp -o Evaluator

dbg:
	$(CC) $(DBGFLAGS) *.cpp -o Evaluator

clean:
	find -name Evaluator -delete
