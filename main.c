#include "attaque.c"
#include <time.h>
//(m1,c1) = (0xe66593, 0xc921bc) (m2,c2) = (0x2fe7c2,0xde877e)
//(m1,c1) = (0xa7cbf0, 0x6e0763) (m2,c2) = (0x8b1e1c,0xe2b54a)

int main()
{
	solution s;
	s = attaque(0xa7cbf0, 0x6e0763, 0x8b1e1c, 0xe2b54a);

	float time2 = clock();
	printf("Nombre de solution: %d\n",s.nombre);
	
	for(int i=0;i<s.nombre;i++)
	{
		printf("Couple de clef %d: %x,%x\n",i,s.clef[i].K1,s.clef[i].K2);
	}

	free(s.clef);
	return 0;
}