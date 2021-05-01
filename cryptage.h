#ifndef CRYPTAGE_H
#define CRYPTAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <inttypes.h>

//La structure uint128_t nous permet de rassembler deux entier non signé de 64 bit pour former un entier de 128 bit
//Dans notre cas les 64 premiers bit du message seront dans la variable d (pour droite) 
//et les 16 derniers dans la variable g (pour gauche)
typedef struct bit_128_struct
{
	uint64_t g;	//16 dernier bit
	uint64_t d;	//64 premier bit
	
} uint128_t;

//La fonction substitution permet de réaliser l'algorithme de substitution suivant le tableau de substitution S
//Entrée :
// -hex : Message binaire de 32 bit max
// -nb_octal: Nombre d'ensemble de 4 bit à permuter (6 pour un message de 24 bit)
//Sortie :
// -Message susbtitué dans un entier non-signé de 32 bit
uint32_t substitution(uint32_t hex, int nb_octal);


//La fonctiton permutation permet de réaliser la permutation suivant le tableau de permutation P
//Entrée :
// -orig : message de 24 bit stocké dans un entier non-signé de 32 bit. Les 8 bit de poid fort sont ignoré
//		   et non utilisé. 
//Sortie :
// -Message de 24 bit permuté dans un entier non-signé de 32 bit.
uint32_t permutation(uint32_t orig);

//La fonction cadencement_clef permet de généré les sous-clef suivant l'algorithme de cadencement de clef
//Entrée :
// -clef_maitre : clef maitre de 24 bit stocké dans un entier non-signé de 64 bit.
//Sortie:
// -Tableau de taille 11 d'entier non-signé de 32 bit contenant les sous-clef généré par la fonction. 
uint32_t* cadencement_clef(uint64_t clef_maitre);

//La fonction cryptage est la fonction principale de la partie cryptage. Elle effectue l'algorithme de cryptage
//Entrée : 
// -message : message binaire de 24 bit stocké dans un entier non-signé de 32 bit 
// -clef : clef de 24 bit permettant de crypter le message, stocké dans un entier non-signé de 64 bit.
//Sortie :
// -Message de 24 bit stocké dans un entier non-signé de 32 bit, crypté dans la fonction.
uint32_t cryptage(uint32_t message, uint64_t clef);


#endif