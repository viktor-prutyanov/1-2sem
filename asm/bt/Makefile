all:
	@g++ -std=gnu++11 -g bt.cpp -Wall -Werror \
	-Wcast-qual -Wcast-align -Wwrite-strings -Winline -Wredundant-decls -Wpointer-arith \
	-o bt && ./bt -d program.bin && echo ''

build:
	@g++ -std=gnu++11 -g bt.cpp -Wall -Werror \
	-Wcast-qual -Wcast-align -Wwrite-strings -Winline -Wredundant-decls -Wpointer-arith \
	-o bt && echo ''

preprocess:
	@g++ -std=gnu++11 -E bt.cpp && echo ''

clean:
	@rm -rf *.o bt