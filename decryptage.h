#ifndef DECRYPTAGE_H
#define DECRYPTAGE_H

#include "cryptage.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <inttypes.h>

//La fonction substitution inverse permet de réaliser l'algorithme de substitution suivant le tableau de sustitution inverse ~S
//Entrée :
// -hex : Message binaire de 32 bit max
// -nb_octal: Nombre d'ensemble de 4 bit à permuter (6 pour un message de 24 bit)
//Sortie :
// -Message susbtitué dans un entier non-signé de 32 bit
uint32_t substitution_inverse(uint32_t hex, int nb_octal);

//La fonctiton permutation inverse permet de réaliser la permutation suivant le tableau de permutation inverse ~P
//Entrée :
// -orig : message de 24 bit stocké dans un entier non-signé de 32 bit. Les 8 bit de poid fort sont ignoré
//		   et non utilisé. 
//Sortie :
// -Message de 24 bit permuté dans un entier non-signé de 32 bit.
uint32_t permutation_inverse(uint32_t orig);

//La fonction decryptage est la fonction principale de la partie decryptage. Elle effectue l'algorithme de decryptage
//Entrée : 
// -message : message binaire crypte de 24 bit stocké dans un entier non-signé de 32 bit 
// -clef : clef de 24 bit permettant de decrypter le message, stocké dans un entier non-signé de 64 bit.
//Sortie :
// -Message de 24 bit stocké dans un entier non-signé de 32 bit, decrypté dans la fonction.
uint32_t decryptage(uint32_t message, uint64_t clef);

#endif