run: main.o
	./main.o

compil: 
	gcc cryptage.c decryptage.c attaque.c main.c -o main.o -O3 -lpthread -g

debug: main.o
	gdb ./main.o

main.o:
	gcc cryptage.c decryptage.c attaque.c main.c -o main.o -O3 -lpthread -g

clean:
	rm *.o -f