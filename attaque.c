#include "attaque.h"

void *contruc_L_1(void * arg_v)
{
	donnee *arg = (donnee*)arg_v;
	for(uint64_t i=0 ; i<(16777216/4); i++)
	{
		arg->K1[i].message = cryptage(*arg->M1, i);
		arg->K1[i].clef = i;
		arg->K2[i].message = decryptage(*arg->C1, i);
		arg->K2[i].clef = i;
	}
	pthread_exit(EXIT_SUCCESS);
}

void *contruc_L_2(void * arg_v)
{
	donnee *arg = (donnee*)arg_v;
	for(uint64_t i=16777216/4; i<16777216/2; i++)
	{
		arg->K1[i].message = cryptage(*arg->M1, i);
		arg->K1[i].clef = i;
		arg->K2[i].message = decryptage(*arg->C1, i);
		arg->K2[i].clef = i;
	}
	pthread_exit(EXIT_SUCCESS);
}

void *contruc_L_3(void * arg_v)
{
	donnee *arg = (donnee*)arg_v;
	for(uint64_t i=16777216/2 ; i<3*16777216/4; i++)
	{
		arg->K1[i].message = cryptage(*arg->M1, i);
		arg->K1[i].clef = i;
		arg->K2[i].message = decryptage(*arg->C1, i);
		arg->K2[i].clef = i;
	}
	pthread_exit(EXIT_SUCCESS);
}

void *contruc_L_4(void * arg_v)
{
	donnee *arg = (donnee*)arg_v;
	for(uint64_t i=3*16777216/4; i<16777216; i++)
	{
		arg->K1[i].message = cryptage(*arg->M1, i);
		arg->K1[i].clef = i;
		arg->K2[i].message = decryptage(*arg->C1, i);
		arg->K2[i].clef = i;
	}
	pthread_exit(EXIT_SUCCESS);
}

void* dicho_1(void *arg_v)
{
	donnee* arg = (donnee*)arg_v;
	for(int i=0;i<16777216/2;i++)
	{
		int deb=0;
		int fin=16777216-1;
		int millieu=fin/2;
		
		int cdt = 0;
		while(!cdt)
		{
			if((arg->K1[i].message<arg->K2[deb].message) || (arg->K1[i].message>arg->K2[fin].message))
			{
				cdt = 1;
				continue;
			}
			if(arg->K1[i].message==arg->K2[millieu].message)
			{
				pthread_mutex_lock(&arg->mutex_dicho);
				arg->s_coll->nombre ++;
				arg->s_coll->clef = reallocarray(arg->s_coll->clef,arg->s_coll->nombre,sizeof(t_clef));
				arg->s_coll->clef[arg->s_coll->nombre-1].K1 = i;
				arg->s_coll->clef[arg->s_coll->nombre-1].K2 = arg->K2[millieu].clef;
				cdt = 1;
				pthread_mutex_unlock(&arg->mutex_dicho);
				continue;
			}
			if(arg->K1[i].message<arg->K2[millieu].message)
			{
				fin = millieu-1;
				millieu = (deb+fin)/2;

			}
			else if(arg->K1[i].message>arg->K2[millieu].message)
			{
				deb = millieu+1;
				millieu = (deb+fin)/2;
			}
		}
	}
	pthread_exit(EXIT_SUCCESS);
}

void* dicho_2(void *arg_v)
{
	donnee* arg = (donnee*)arg_v;
	for(int i=16777216/2;i<16777216;i++)
	{
		int deb=0;
		int fin=16777216-1;
		int millieu=fin/2;
		
		int cdt = 0;
		while(!cdt)
		{
			if((arg->K1[i].message<arg->K2[deb].message) || (arg->K1[i].message>arg->K2[fin].message))
			{
				cdt = 1;
				continue;
			}
			if(arg->K1[i].message==arg->K2[millieu].message)
			{
				pthread_mutex_lock(&arg->mutex_dicho);
				arg->s_coll->nombre ++;
				arg->s_coll->clef = reallocarray(arg->s_coll->clef,arg->s_coll->nombre,sizeof(t_clef));
				arg->s_coll->clef[arg->s_coll->nombre-1].K1 = i;
				arg->s_coll->clef[arg->s_coll->nombre-1].K2 = arg->K2[millieu].clef;
				cdt = 1;
				pthread_mutex_unlock(&arg->mutex_dicho);
				continue;
			}
			if(arg->K1[i].message<arg->K2[millieu].message)
			{
				fin = millieu-1;
				millieu = (deb+fin)/2;

			}
			else if(arg->K1[i].message>arg->K2[millieu].message)
			{
				deb = millieu+1;
				millieu = (deb+fin)/2;
			}
		}
	}
	pthread_exit(EXIT_SUCCESS);
}



void* test_1(void * arg_v)
{
	donnee *arg = (donnee*)arg_v;
	uint32_t crypt;
	uint32_t decrypt;
	for(int i=0;i<(arg->s_coll->nombre/4);i++)
	{
		crypt = cryptage(*arg->M2,arg->s_coll->clef[i].K1);
		decrypt = decryptage(*arg->C2,arg->s_coll->clef[i].K2);

		if(crypt == decrypt)
		{
			pthread_mutex_lock(&arg->mutex_dicho);
			arg->s_test->nombre ++;
			arg->s_test->clef = reallocarray(arg->s_test->clef, arg->s_test->nombre, sizeof(t_clef));
			arg->s_test->clef[arg->s_test->nombre-1].K1 = arg->s_coll->clef[i].K1;
			arg->s_test->clef[arg->s_test->nombre-1].K2 = arg->s_coll->clef[i].K2;
			pthread_mutex_unlock(&arg->mutex_dicho);
		}
	}
	pthread_exit(EXIT_SUCCESS);
}

void* test_2(void * arg_v)
{
	donnee *arg = (donnee*)arg_v;
	uint32_t crypt;
	uint32_t decrypt;
	for(int i=arg->s_coll->nombre/4;i<(arg->s_coll->nombre/2);i++)
	{
		crypt = cryptage(*arg->M2,arg->s_coll->clef[i].K1);
		decrypt = decryptage(*arg->C2,arg->s_coll->clef[i].K2);

		if(crypt == decrypt)
		{
			pthread_mutex_lock(&arg->mutex_dicho);
			arg->s_test->nombre ++;
			arg->s_test->clef = reallocarray(arg->s_test->clef,arg->s_test->nombre,sizeof(t_clef));
			arg->s_test->clef[arg->s_test->nombre-1].K1 = arg->s_coll->clef[i].K1;
			arg->s_test->clef[arg->s_test->nombre-1].K2 = arg->s_coll->clef[i].K2;
			pthread_mutex_unlock(&arg->mutex_dicho);
		}
	}
	pthread_exit(EXIT_SUCCESS);
}

void* test_3(void * arg_v)
{
	donnee *arg = (donnee*)arg_v;
	uint32_t crypt;
	uint32_t decrypt;
	for(int i=arg->s_coll->nombre/2;i<((3*arg->s_coll->nombre)/4);i++)
	{
		crypt = cryptage(*arg->M2,arg->s_coll->clef[i].K1);
		decrypt = decryptage(*arg->C2,arg->s_coll->clef[i].K2);

		if(crypt == decrypt)
		{
			pthread_mutex_lock(&arg->mutex_dicho);
			arg->s_test->nombre ++;
			arg->s_test->clef = reallocarray(arg->s_test->clef,arg->s_test->nombre,sizeof(t_clef));
			arg->s_test->clef[arg->s_test->nombre-1].K1 = arg->s_coll->clef[i].K1;
			arg->s_test->clef[arg->s_test->nombre-1].K2 = arg->s_coll->clef[i].K2;
			pthread_mutex_unlock(&arg->mutex_dicho);
		}
	}
	pthread_exit(EXIT_SUCCESS);
}

void* test_4(void * arg_v)
{
	donnee *arg = (donnee*)arg_v;
	uint32_t crypt;
	uint32_t decrypt;
	for(int i=(3*arg->s_coll->nombre)/4;i<arg->s_coll->nombre;i++)
	{
		crypt = cryptage(*arg->M2,arg->s_coll->clef[i].K1);
		decrypt = decryptage(*arg->C2,arg->s_coll->clef[i].K2);

		if(crypt == decrypt)
		{
			pthread_mutex_lock(&arg->mutex_dicho);
			arg->s_test->nombre ++;
			arg->s_test->clef = reallocarray(arg->s_test->clef,arg->s_test->nombre,sizeof(t_clef));
			arg->s_test->clef[arg->s_test->nombre-1].K1 = arg->s_coll->clef[i].K1;
			arg->s_test->clef[arg->s_test->nombre-1].K2 = arg->s_coll->clef[i].K2;
			pthread_mutex_unlock(&arg->mutex_dicho);
		}
	}
	pthread_exit(EXIT_SUCCESS);
}

donnee init_arg(uint32_t *m1, uint32_t *c1, uint32_t *m2, uint32_t *c2)
{
	donnee arg;

	pthread_mutex_init(&arg.mutex_dicho,NULL);

	arg.M1=m1;
	arg.C1=c1;
	arg.M2=m2;
	arg.C2=c2;

	arg.K1 = malloc(16777216*sizeof(message_clef));
	arg.K2 = malloc(16777216*sizeof(message_clef));

	arg.s_test = malloc(sizeof(solution));

	arg.s_coll = malloc(sizeof(solution));
	arg.s_coll->clef = malloc(1*sizeof(t_clef));
	arg.s_coll->nombre = 0;
	arg.s_test->clef = malloc(1*sizeof(t_clef));
	arg.s_test->nombre = 0;
	
	return arg;
}

void freemem(donnee arg)
{
	pthread_mutex_destroy(&arg.mutex_dicho);

	free(arg.K1);
	free(arg.K2);

	free(arg.s_test->clef);
	free(arg.s_test);

	free(arg.s_coll->clef);
	free(arg.s_coll);
}

int cmp(const void * a, const void * b)
{
	const message_clef* a_mc = (message_clef*)a;
	const message_clef* b_mc = (message_clef*)b;
	return ( a_mc->message - b_mc->message );
}

solution copy(solution orig)
{
	solution s;
	s.nombre = orig.nombre;
	s.clef = malloc(s.nombre*sizeof(t_clef));
	for(int i=0; i<s.nombre;i++)
	{
		s.clef[i].K1 = orig.clef[i].K1;
		s.clef[i].K2 = orig.clef[i].K2;
	}
	return s;
}

solution attaque(uint32_t m1, uint32_t c1, uint32_t m2, uint32_t c2)
{
	//#Calcul des clefs
	
	donnee arg = init_arg(&m1, &c1, &m2, &c2);
	pthread_t contrucL1;
	pthread_t contrucL2;
	pthread_t contrucL3;
	pthread_t contrucL4;

	pthread_t dichotomie1;
	pthread_t dichotomie2;

	pthread_t test1;
	pthread_t test2;
	pthread_t test3;
	pthread_t test4;
	
	int verif;
	verif = pthread_create(&contrucL1, NULL, contruc_L_1, &arg);
	verif += pthread_create(&contrucL2, NULL, contruc_L_2, &arg);
	verif += pthread_create(&contrucL3, NULL, contruc_L_3, &arg);
	verif += pthread_create(&contrucL4, NULL, contruc_L_4, &arg);
	if(verif !=0)
	{
		printf("Erreur Fatale: Un thread n'a pas pu ??tre cr??er\n");
		exit(1);
	}
	verif = 0;
	pthread_join(contrucL1,NULL);
	pthread_join(contrucL2,NULL);
	pthread_join(contrucL3,NULL);
	pthread_join(contrucL4,NULL);
	printf("Fin de la construction des listes Lm et Lc...\n");
	
	//#Test des clefs
	//##Trie la listes des mots d??crypter.
	
	qsort(arg.K2,16777216,sizeof(message_clef),cmp);	

	//##Recherche des ??l??ments commun

	verif = pthread_create(&dichotomie1, NULL, dicho_1, &arg);
	verif += pthread_create(&dichotomie2, NULL, dicho_2, &arg);
	if(verif !=0)
	{
		printf("Erreur Fatale: Un thread n'a pas pu ??tre cr??er\n");
		exit(2);
	}

	pthread_join(dichotomie1,NULL);
	pthread_join(dichotomie2,NULL);
	
	printf("Fin de la recherche des ??l??ments commun de Lm et Lc...\n");
	
	//##Test des clefs pour trouver le couples de clef cherch??s

	verif = pthread_create(&test1, NULL, test_1, &arg);
	verif += pthread_create(&test2, NULL, test_2, &arg);
	verif += pthread_create(&test3, NULL, test_3, &arg);
	verif += pthread_create(&test4, NULL, test_4, &arg);
	if(verif !=0)
	{
		printf("Erreur Fatale: Un thread n'a pas pu ??tre cr??er\n");
		exit(3);
	}

	pthread_join(test1,NULL);
	pthread_join(test2,NULL);
	pthread_join(test3,NULL);
	pthread_join(test4,NULL);
	printf("Fin des test des clef\n");
	
	solution s;
	s = copy(*arg.s_test);
	freemem(arg);
	return s;
}