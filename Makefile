scanner: scanner.l
	flex -o scanner.cpp scanner.l
	g++ -o scanner scanner.cpp

clean:
	rm scanner.cpp scanner
