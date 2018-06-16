

CC = c++


test:	fc
	${CC} -g test.cpp fc.o -o test

fc:
	${CC} -c fc.cpp -o fc.o

clean:
	rm test
