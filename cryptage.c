#include "cryptage.h"

uint32_t substitution(uint32_t hex, int nb_octal)
{
	uint32_t new_hex=0x0;
	uint32_t mask = 0xf;
	uint32_t S[] = {0xc, 0x5, 0x6, 0xb, 0x9, 0x0, 0xa, 0xd, 0x3, 0xe, 0xf, 0x8, 0x4, 0x7, 0x1, 0x2};
	for(int i=0;i<nb_octal;i++)
	{
		uint32_t tmp = S[(hex&mask)>>(4*i)];
		tmp = tmp << (4*i);
		new_hex = new_hex^tmp;
		mask = mask << 4;
	}
	return new_hex;
}

uint32_t permutation(uint32_t orig)
{
	uint32_t crypt = 0x0;
	uint32_t mask = 0x1;
	crypt = crypt^(orig&mask);
	crypt = crypt^(orig&(mask<<23));
	int decallage = 0;
	for(int i=1; i<23;i++)
	{
		decallage += 6;
		decallage = decallage % 23;
		mask = mask<<1;
		uint32_t tmp = 0x0;
		tmp = (orig&mask)>>i;
		tmp = tmp<<(decallage);
		crypt = crypt^tmp;
	}
	return crypt;
}

uint32_t* cadencement_clef(uint64_t clef_maitre)
{
	uint32_t* K = malloc(11*sizeof(uint32_t));
	uint128_t clef; clef.d = 0x0; clef.g = 0x0;
	
	//Insertion de la clef maitre---------------------
	
	uint128_t mask_insertion_clef_maitre;
	mask_insertion_clef_maitre.g = 0xffff00;
	mask_insertion_clef_maitre.d = 0xff;
	clef.g = (mask_insertion_clef_maitre.g&clef_maitre)>>8;
	clef.d = (mask_insertion_clef_maitre.d&clef_maitre)<<56;

	for(int i=1;i<12;i++)
	{
		//Récupération de la sous clef
		
		K[i]=(uint32_t)((clef.d&0xffffff0000)>>16);
		
		//Décallage de la clef----------------------------
		
		uint64_t mask_extraction_d = 0x7ffff;
		uint64_t bit0_18 = clef.d&mask_extraction_d;

		uint64_t sauvegarde_d;
		sauvegarde_d = clef.d&(~mask_extraction_d);	//sauvegarde la partie droite sans les bit décallés
		sauvegarde_d = sauvegarde_d>>19;			//décallage de la partie droite
		clef.d = clef.g<<48;						//insertion des 16 bit de gauche à gauche
		clef.d = clef.d>>3;							//Supression des 3 bits restant + décallage des bit de gauche a leur position repective (61e bit)
		clef.d = clef.d^sauvegarde_d;				//Insertion des bit de droite déja décallé
		clef.d = clef.d^((bit0_18&0x7)<<61);		//Insertion des 3 dernier bit de la partie droite
		clef.g=bit0_18>>3;							//Insertion des 16 bit de gauche
		
		//Substitution des 4 derniers bits de gauche------- 
		
		uint32_t sub;
		sub = substitution((uint32_t)((clef.g&0xf000)>>12),1);
		clef.g = (clef.g&0x0fff);
		clef.g = clef.g^((uint64_t)(sub<<12));

		//xor des bits 15 à 19 avec i

		clef.d = clef.d^(i<<15);
	}
		return K;
}


uint32_t cryptage(uint32_t message, uint64_t clef)
{
	
	uint32_t *K=NULL;
	K=cadencement_clef(clef);

	for(int i=1;i<11;i++)
	{
		message = message^K[i];
		message = substitution(message,6);
		message = permutation(message);
	}
	message = message^K[11];
	free(K);
	return message;
}
