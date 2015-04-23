all: print.o
	@gcc print.o -o test && ./test && echo ''

print.o: print.asm
	@nasm -f elf64 print.asm

clean:
	@rm -rf *.o test