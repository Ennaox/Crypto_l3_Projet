run: compil
	./main.o

perf: compil
	gprof main.o

memcheck: compil
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --error-limit=no ./main.o

debug: compil
	gdb ./main.o
compil: clean
	gcc main.c -o main.o -O3 -lpthread -g -pg
clean:
	rm *.o -f