all: shell


shell:	main.c parse.c parse.h
	gcc main.c parse.c -o shell
clean:
	rm -f shell *~

