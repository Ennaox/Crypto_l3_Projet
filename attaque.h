#ifndef ATTAQUE_H
#define ATTAQUE_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <inttypes.h>
#include <pthread.h>

#include "cryptage.h"
#include "decryptage.h"

//La structure t_clef permet de stocker les couples de clef K1/K2
typedef struct clef_T{
	uint32_t K1;		//clef de 24 bit stocké dans un entier non-signé de 32 bit 
	uint32_t K2;		//clef de 24 bit stocké dans un entier non-signé de 32 bit 
} t_clef;

//La structure solution permet de stocké les couples de clef solution K1/K2
typedef struct s{
	t_clef *clef;		//Tableau, de taille nombre, de couple de clef K1/K2
	uint32_t nombre;	//Nombre de couple de clef dans le tableau clef[]
} solution;

//La structure message_clef permet de stocker une clef ainsi que le message qu'elle a généré.
typedef struct message_clef
{
	uint32_t clef;		//clef binaire stocké dans un entier non-signé de 32 bit.
	uint32_t message;	//message bianire stocké dans un entier non-signé de 32 bit.
} message_clef;


//La structure donnee nous permet de stocké tout les arguments nécéssaire aux différents thread utilisé .
typedef struct donnee_{
	pthread_mutex_t mutex_dicho;	//Mutex servant pour verrouiller nos thread dico_1 et dico_2.

	uint32_t *M1;	//Pointeur vers le message binaire de 24 bit M1 stocké dans un entier non-signé de 32 bit.
	uint32_t *C1;	//Pointeur vers le message crypté binaire de 24 bit C1 stocké dans un entier non-signé de 32 bit.

	uint32_t *M2;	//Pointeur vers le message binaire de 24 bit M2 stocké dans un entier non-signé de 32 bit.
	uint32_t *C2;	//Pointeur vers le message crypté binaire de 24 bit C2 stocké dans un entier non-signé de 32 bit.

	message_clef* K1;	//Tableau de taille 16 777 216 stockant les résultats du cryptage du message M1.
	message_clef* K2;	// Tableau de taille 16 777 216 stockabt les résultats du decryptage du message crypté C1.

	solution *s_coll;	//Tableau des résultats des comparaison entre K1 et K2.

	solution *s_test1;	//Tableau des résultats des tests des clef.
	solution *s_test2;	//Tableau des résultats des tests des clef.
	solution *s_test3;	//Tableau des résultats des tests des clef.
	solution *s_test4;	//Tableau des résultats des tests des clef.
} donnee;

//Les fonctions cryptage permettent de crypté le message M1 et de décrypter C1 avec tout les clef possible.
//Entrée : 
// -arg_v : pointeur vers la struct donnee.
//Sortie: 
// -Les fonctions suivantes complètent la structure de donnée en remplissant K1 et K2.
//
//Les fonctions suivantes sont itilisés pour générer des threads.
void *contruc_L_1(void * arg_v);		//Cryptages de M1 et décryptage de C1 avec les clefs allant de 0 à 16777216/4.
void *contruc_L_2(void * arg_v);		//Cryptages de M1 et décryptage de C1 avec les clefs allant de 16777216/4 à 16777216/2.
void *contruc_L_3(void * arg_v);		//Cryptages de M1 et décryptage de C1 avec les clefs allant de 16777216/2 à 3*16777216/4.
void *contruc_L_4(void * arg_v);		//Cryptages de M1 et décryptage de C1 avec les clefs allant de 3*16777216/4 à 16777216.

//Les fonction dicho (pour dichotomie) permettent de comparer K1 avec K2 et de renvoyer les clefs et les messages où K1=K2.
//Ces fonction réalise un algorithme diviser pour mieux régner pour rechercher les cas ou K1=K2.
//Entrée :
// -arg_v : pointeur vers la struct donnee.
//Sortie :
// -Les fonctions suivantes complètent la structure de donnée en remplissant s_coll.
//
//Les fonctions suivantes sont itilisés pour générer des threads.
void* dicho_1(void *arg_v) ;
void* dicho_2(void *arg_v);

//Les fonction test permettent de comparer tester les clefs obtenu par la comparaison de K1 et K2 stocké dans le tableau s_coll,
// en cryptant M2 avec K1, en decryptant C2 avec K2 et en comparant les deux mots obtenu. 
//Entrée :
// -arg_v : pointeur vers la struct donnee.
//Sortie :
// -Les fonctions suivantes complètent la structure de donnée en remplissant respectivement les tableaux s_test1, s_test2, s_test3
//  et s_test4.
//
//Les fonctions suivantes sont itilisés pour générer des threads.
void* test_1(void * arg_v);
void* test_2(void * arg_v);
void* test_3(void * arg_v);
void* test_4(void * arg_v);

//La fonction init_arg permet d'initialiser la structure donnee notamment en alouant de la mémoire aux différents pointeur
// ainsi qu'en replissant avec les donnée initiale.
//Entrée :
// -pointeur vers m1, message 1.
// -pointeur vers c1, crypté 1.
// -pointeur vers m2, message 2.
// -pointeur vers c2, crypté 2.
//Sortie:
// -Structure donnee initialisé par la fonction.
donnee init_arg(uint32_t *m1, uint32_t *c1, uint32_t *m2, uint32_t *c2);

//La fonction freemem permet de désalouer la mémoire alloué pour le contenu de la structure donnée
//Entrée :
// -arg : structure donnee a vider.
void freemem(donnee arg);

//Fonction de comparaison utiliser par la fonction standard qsort().
//Entrée :
// -a : pointeur vers une strucutre message_clef
// -b : pointeur vers une strucutre message_clef
//Sortie :
// -Résultat de la soustraction: a_mc->message - b_mc->message
int cmp(const void * a, const void * b);

//Fonction fusion permet de fusionné les strucutes s_test1, s_test2, s_test3, s_test4 en une seul structure solution.
//Entrée :
// -arg : strucutre donnee
//Sortie :
// -Strucute solution résultant de la fusion des tableaux s_test
solution fusion(donnee arg);

//Fonction principale de l'attaque par le milieu. Elle permet de réaliser l'attaque et de renvoyer la/les solutions.
//Entrée :
// -pointeur vers m1, message 1.
// -pointeur vers c1, crypté 1.
// -pointeur vers m2, message 2.
// -pointeur vers c2, crypté 2.
//Sortie :
// -Structure solution contenant les tableaux
solution attaque(uint32_t m1, uint32_t c1, uint32_t m2, uint32_t c2);
#endif