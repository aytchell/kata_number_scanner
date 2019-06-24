all: scanner

H_FILES = scanner.h
CXX_FILES = main.cpp scanner.cpp

scanner: $(H_FILES) $(CXX_FILES)
	g++ -std=c++11 -Wall -Wextra -pedantic -o scanner $(CXX_FILES)
