all: fpu_test.o
	@gcc fpu_test.o -o fpu_test && ./fpu_test && echo ''

fpu_test.o: fpu_test.asm
	@nasm -f elf64 fpu_test.asm

clean:
	@rm -rf *.o fpu_test