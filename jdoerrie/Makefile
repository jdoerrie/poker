CC=g++
CXXFLAGS=-std=c++14 -Wall -Wextra -Werror -pedantic
LIBS=-lboost_regex
OPTFLAGS=-O2
DBGFLAGS=-O0 -g

all:
	mkdir -p out/Release/
	$(CC) $(CXXFLAGS) $(OPTFLAGS) src/*.cpp -o out/Release/Evaluator $(LIBS)

dbg:
	mkdir -p out/Debug/
	$(CC) $(CXXFLAGS) $(DBGFLAGS) src/*.cpp -o out/Debug/Evaluator $(LIBS)

clean:
	rm -rf out/
