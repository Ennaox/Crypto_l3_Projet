run: compil

test: compil
	./main.o 000000 000000
compil: clean
	gcc main.c -o main.o
clean:
	rm *.o -f