run: main.o
	./main.o

compil: 
	gcc cryptage.c decryptage.c attaque.c main.c -o main.o -O3 -lpthread -g -Wall

debug: main.o
	gdb ./main.o

main.o:
	gcc cryptage.c decryptage.c attaque.c main.c -o main.o -O3 -lpthread -g

verif: verif.o
	./verif.o

verif.o:
	gcc cryptage.c decryptage.c attaque.c verif.c -o verif.o -O3 -lpthread -g

clean:
	rm *.o -f