all: main.o myprintf.o
	@gcc -o test main.o myprintf.o && ./test && echo ''

main.o: main.c
	@gcc -c main.c

myprintf.o: myprintf.asm
	@nasm -f elf64 myprintf.asm

clean:
	@rm -rf *.o test