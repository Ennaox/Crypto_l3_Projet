#include "cryptage.c"

int main(int argc, char const *argv[])
{
	if(argc !=3)
	{
		printf("Erreur: nombre d'argument invalide\n");
		printf("Usage: ./main.o *message* *clef*\n");
		return 1;
	}

	uint64_t clef_maitre = strtol(argv[1],NULL,16);
	uint32_t message = strtol(argv[2],NULL,16);
	
	uint32_t cryptee = cryptage(message ,clef_maitre);
	printf("%x %x %x\n",message, clef_maitre, cryptee);
	return 0;
}