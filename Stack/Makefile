all:
	@g++ -std=gnu++11 -g test.cpp -Wall \
	-Wcast-qual -Wcast-align -Wwrite-strings -Winline -Wredundant-decls -Wpointer-arith \
	-o test && ./test && echo ''

build:
	@g++ -std=gnu++11 -g test.cpp -Wall \
	-Wcast-qual -Wcast-align -Wwrite-strings -Winline -Wredundant-decls -Wpointer-arith \
	-o test

preprocess:
	@g++ -std=gnu++11 -E test.cpp && echo ''

memcheck:
	@valgrind --track-origins=yes --show-leak-kinds=all --leak-check=full ./test

clean:
	@rm -rf *.o test *.txt