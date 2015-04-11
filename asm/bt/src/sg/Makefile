all: generate

generate:
	@nasm -f bin snippet.asm -o snippet.bin && \
	../../bin/bin2hex > snippet.hex && \
	xclip -selection clipboard < snippet.hex &&\
	cat snippet.hex

clean:
	@rm -rf *.bin *.hex