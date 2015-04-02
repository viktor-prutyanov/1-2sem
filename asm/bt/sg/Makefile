all: generate

generate:
	@nasm -f bin snippet.asm -o snippet.bin && \
	./bin2hex -x > snippet.hex && \
	xclip -selection clipboard < snippet.hex &&\
	cat snippet.hex