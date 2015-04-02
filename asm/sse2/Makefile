all: test.o
	@gcc test.o -o test  && ./test && echo ''

test.o: test.asm
	@nasm -f elf64 test.asm

clean:
	@rm -rf *.o test