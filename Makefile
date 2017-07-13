all:bfi

bfi:
	gcc -O2 -Wall -Wextra bfi.c -o bfi

test:bfi
	./bfi helloworld.bf

.PHONY:clean

clean:
	rm ./bfi