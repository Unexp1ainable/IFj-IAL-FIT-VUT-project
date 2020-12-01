CFLAGS=-g -Wall -Wextra 
FILES=main.c dynamic_string.c pars_string.c newscanner.c

main: $(FILES)
	gcc $(FILES) -o main $(CFLAGS)

syntax: main_syntax.c syntax.c expression.c dynamic_string.c pars_string.c newscanner.c
	gcc $^ -o main_s.out $(CFLAGS)
