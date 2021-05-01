#include "cryptage.h"
#include "decryptage.h"
#include "attaque.h"

int main()
{
	time_t deb,fin;
	deb = time(NULL);
	printf("Début de l'attaque...\n");
	solution s;
	s = attaque(0xa7cbf0, 0x6e0763, 0x8b1e1c, 0xe2b54a);

	printf("\nNombre de solution: %d\n",s.nombre);
	
	for(int i=0;i<s.nombre;i++)
	{
		printf("Couple de clef %d: %x,%x\n",i,s.clef[i].K1,s.clef[i].K2);
	}

	free(s.clef);
	fin = time(NULL);
	printf("Temps d'éxécution: %d sec\n",fin-deb );
	return 0;
}