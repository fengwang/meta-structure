all: test.cpp
	g++ -o test test.cpp -std=c++20 -O2 -Wall -Wextra
