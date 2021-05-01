#include "decryptage.h"

uint32_t substitution_inverse(uint32_t hex, int nb_octal)
{
	uint32_t new_hex=0x0;
	uint32_t mask = 0xf;
	uint32_t S[] = {0x5, 0xe, 0xf, 0x8, 0xc, 0x1, 0x2, 0xd, 0xb, 0x4, 0x6, 0x3, 0x0, 0x7, 0x9, 0xa};
	for(int i=0;i<nb_octal;i++)
	{
		uint32_t tmp = S[(hex&mask)>>(4*i)];
		tmp = tmp << (4*i);
		new_hex = new_hex^tmp;
		mask = mask << 4;
	}
	return new_hex;
}

uint32_t permutation_inverse(uint32_t orig)
{
	uint32_t crypt = 0x0;
	uint32_t mask = 0x1;
	crypt = crypt^(orig&mask);
	crypt = crypt^(orig&(mask<<23));
	int decallage = 0;
	for(int i=1; i<23;i++)
	{
		mask = 0x1;
		decallage += 6;
		decallage = decallage % 23;
		mask=mask<<decallage;
		uint32_t tmp = 0x0;
		tmp = (orig&mask)>>decallage;
		tmp = tmp<<(i);
		crypt = crypt^tmp;
	}
	return crypt;
}

uint32_t decryptage(uint32_t message, uint64_t clef)
{
	uint32_t *K=NULL;
	K=cadencement_clef(clef);
	message = message^K[11];
	for(int i=10;i>0;i--)
	{
		message = permutation_inverse(message);
		message = substitution_inverse(message,6);
		message = message^K[i];
	}
	free(K);
	return message;

}