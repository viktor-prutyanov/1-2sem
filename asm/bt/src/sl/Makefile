all:
	@g++ -std=gnu++11 -g sl.cpp -Wall \
	-Wcast-qual -Wcast-align -Wwrite-strings -Winline -Wredundant-decls -Wpointer-arith \
	-o ../../bin/sl && ../../bin/sl -d -i ../../examples/factorial.obj -o ../../bin/factorial  && echo ''

build:
	@g++ -std=gnu++11 -g sl.cpp -Wall \
	-Wcast-qual -Wcast-align -Wwrite-strings -Winline -Wredundant-decls -Wpointer-arith \
	-o ../../bin/sl

preprocess:
	@g++ -std=gnu++11 -E sl.cpp && echo ''

memcheck:
	@valgrind --track-origins=yes --show-leak-kinds=all --leak-check=full \
	../../bin/sl -i  ../../examples/factorial.obj -o ../../examples/factorial

clean:
	@rm -rf *.o sl *.txt