all: parser.h scanner.cpp main.cpp
	g++ -o typhon *.cpp -std=c++11

scanner.cpp: scanner.l parser.h
	flex -o scanner.cpp scanner.l

parser.h: parser.y
	bison --defines=parser.h -o parser.cpp parser.y

clean:
	rm parser.cpp parser.h scanner.cpp typhon
