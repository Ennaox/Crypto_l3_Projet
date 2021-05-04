#include "cryptage.h"
#include "decryptage.h"
#include "attaque.h"

int main(int argc, char const *argv[])
{
	solution s;
	uint32_t m1= 0xa7cbf0;
	uint32_t c1= 0x6e0763;
	uint32_t m2= 0x8b1e1c;
	uint32_t c2= 0xe2b54a;
	uint32_t mot;
	printf("Attaque en cours...\n");
	s = attaque(m1, c1, m2, c2);
	printf("Fin de l'attaque\n\n");

	for(int i=0;i<s.nombre;i++)
	{
		printf("Couple clef: %x, %x\n",s.clef[i].K1,s.clef[i].K2 );
		mot = cryptage(m1,s.clef[i].K1);
		mot = cryptage(mot,s.clef[i].K2);
		
		printf("m1 crypté: %x au lieu de %x\n",mot,c1);
		mot = decryptage(c1,s.clef[i].K2);
		mot = decryptage(mot,s.clef[i].K1);
		printf("c1 décrypté: %x au lieu de %x\n",mot,m1);

		mot = cryptage(m2,s.clef[i].K1);
		mot = cryptage(mot,s.clef[i].K2);
		
		printf("m2 crypté: %x au lieu de %x\n",mot,c2);
		mot = decryptage(c2,s.clef[i].K2);
		mot = decryptage(mot,s.clef[i].K1);
		printf("c2 décrypté: %x au lieu de %x\n\n",mot,m2);
	}

	return 0;
}