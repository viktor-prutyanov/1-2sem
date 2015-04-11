all:
	@g++ -std=gnu++11 -g bt.cpp -Wall -Werror \
	-Wcast-qual -Wcast-align -Wwrite-strings -Winline -Wredundant-decls -Wpointer-arith \
	-o ../../bin/bt && ../../bin/bt -n -i ../../examples/factorial.bin -o ../../examples/factorial.obj  && echo ''

build:
	@g++ -std=gnu++11 -g bt.cpp -Wall -Werror \
	-Wcast-qual -Wcast-align -Wwrite-strings -Winline -Wredundant-decls -Wpointer-arith \
	-o ../../bin/bt

preprocess:
	@g++ -std=gnu++11 -E bt.cpp && echo ''

clean:
	@rm -rf *.o bt *.txt