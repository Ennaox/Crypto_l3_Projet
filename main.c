#include "attaque.c"
#include <time.h>

int main(int argc, char const *argv[])
{
	float time = clock();
	if(argc !=3)
	{
		printf("Erreur: nombre d'argument invalide\n");
		printf("Usage: ./main.o *message* *clef*\n");
		return 1;
	}
	
	uint128_t yes;
	uint64_t clef_maitre = strtol(argv[2],NULL,16);
	uint32_t message = strtol(argv[1],NULL,16);
	uint32_t cryptee = cryptage(message ,clef_maitre);
	printf("message clair: %x; clef maitre: %x; message crypte: %x\n",message, clef_maitre, cryptee);
	
	message = decryptage(cryptee, clef_maitre);
	printf("message crypte: %x; clef maitre: %x; message clair: %x\n",cryptee, clef_maitre, message);
	float time2 = clock();
	printf("\n temps d'éxécution: %f\n",(time2 - time)/CLOCKS_PER_SEC);
	return 0;
}