#include "decryptage.c"
#include <pthread.h>

typedef struct clef_T{
	uint32_t K1;
	uint32_t K2;
} t_clef;

typedef struct s{
	t_clef *clef;
	uint32_t nombre;
} solution;

typedef struct message_clef
{
	uint32_t clef;
	uint32_t message;
} message_clef;

typedef struct donnee_{
	
	pthread_mutex_t mutex_dicho;

	uint32_t *M1;
	uint32_t *C1;

	uint32_t *M2;
	uint32_t *C2;

	message_clef* K1;
	message_clef* K2;

	solution *s_coll;
	
	solution *s_dicho;

	solution *s_test1;
	solution *s_test2;
	solution *s_test3;
	solution *s_test4;
} donnee;

void *cryptage_1(void * arg_v)
{
	donnee *arg = (donnee*)arg_v;
	for(uint64_t i=0 ; i<(16777216/2); i++)
	{
		arg->K1[i].message = cryptage(*arg->M1, i);
		arg->K1[i].clef = i;
	}
	printf("Thread 1 fini\n");
	pthread_exit(EXIT_SUCCESS);
}

void *cryptage_2(void * arg_v)
{
	donnee *arg = (donnee*)arg_v;
	for(uint64_t i=16777216/2; i<(16777216); i++)
	{
		arg->K1[i].message = cryptage(*arg->M1, i);
		arg->K1[i].clef = i;
	}
	printf("Thread 2 fini\n");
	pthread_exit(EXIT_SUCCESS);
}

void *decryptage_1(void * arg_v)
{
	donnee *arg = (donnee*)arg_v;
	for(uint64_t i=0 ; i<(16777216/2); i++)
	{
		arg->K2[i].message = decryptage(*arg->C1, i);
		arg->K2[i].clef = i;
	}
	printf("Thread 3 fini\n");
	pthread_exit(EXIT_SUCCESS);
}

void *decryptage_2(void * arg_v)
{
	donnee *arg = (donnee*)arg_v;
	for(uint64_t i=16777216/2; i<(16777216); i++)
	{
		arg->K2[i].message = decryptage(*arg->C1, i);
		arg->K2[i].clef = i;
	}
	printf("Thread 4 fini\n");
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
	printf("Thread 1 fini\n");
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
	printf("Thread 2 fini\n");
	pthread_exit(EXIT_SUCCESS);
}



void* test_1(void * arg_v)
{
	donnee *arg = (donnee*)arg_v;
	arg->s_test1->clef = malloc(1*sizeof(t_clef));
	uint32_t crypt;
	uint32_t decrypt;
	arg->s_test1->nombre = 0;
	for(int i=0;i<(arg->s_coll->nombre/4);i++)
	{
		crypt = cryptage(*arg->M2,arg->s_coll->clef[i].K1);
		decrypt = decryptage(*arg->C2,arg->s_coll->clef[i].K2);

		if(crypt == decrypt)
		{
			arg->s_test1->nombre ++;
			arg->s_test1->clef = reallocarray(arg->s_test1->clef, arg->s_test1->nombre, sizeof(t_clef));
			arg->s_test1->clef[arg->s_test1->nombre-1].K1 = arg->s_coll->clef[i].K1;
			arg->s_test1->clef[arg->s_test1->nombre-1].K2 = arg->s_coll->clef[i].K2;
		}
	}
	printf("Thread 1 fini\n");
	pthread_exit(EXIT_SUCCESS);
}

void* test_2(void * arg_v)
{
	donnee *arg = (donnee*)arg_v;
	arg->s_test2->clef = malloc(1*sizeof(t_clef));
	uint32_t crypt;
	uint32_t decrypt;
	arg->s_test2->nombre = 0;
	for(int i=arg->s_coll->nombre/4;i<(arg->s_coll->nombre/2);i++)
	{
		crypt = cryptage(*arg->M2,arg->s_coll->clef[i].K1);
		decrypt = decryptage(*arg->C2,arg->s_coll->clef[i].K2);

		if(crypt == decrypt)
		{
			arg->s_test2->nombre ++;
			arg->s_test2->clef = reallocarray(arg->s_test2->clef,arg->s_test2->nombre,sizeof(t_clef));
			arg->s_test2->clef[arg->s_test2->nombre-1].K1 = arg->s_coll->clef[i].K1;
			arg->s_test2->clef[arg->s_test2->nombre-1].K2 = arg->s_coll->clef[i].K2;
		}
	}
	printf("Thread 2 fini\n");
	pthread_exit(EXIT_SUCCESS);
}

void* test_3(void * arg_v)
{
	donnee *arg = (donnee*)arg_v;
	arg->s_test3->clef = malloc(1*sizeof(t_clef));
	uint32_t crypt;
	uint32_t decrypt;
	arg->s_test3->nombre = 0;
	for(int i=arg->s_coll->nombre/2;i<((3*arg->s_coll->nombre)/4);i++)
	{
		crypt = cryptage(*arg->M2,arg->s_coll->clef[i].K1);
		decrypt = decryptage(*arg->C2,arg->s_coll->clef[i].K2);

		if(crypt == decrypt)
		{
			arg->s_test3->nombre ++;
			arg->s_test3->clef = reallocarray(arg->s_test3->clef,arg->s_test3->nombre,sizeof(t_clef));
			arg->s_test3->clef[arg->s_test3->nombre-1].K1 = arg->s_coll->clef[i].K1;
			arg->s_test3->clef[arg->s_test3->nombre-1].K2 = arg->s_coll->clef[i].K2;
		}
	}
	printf("Thread 3 fini\n");
	pthread_exit(EXIT_SUCCESS);
}

void* test_4(void * arg_v)
{
	donnee *arg = (donnee*)arg_v;
	arg->s_test4->clef = malloc(1*sizeof(t_clef));
	uint32_t crypt;
	uint32_t decrypt;
	arg->s_test4->nombre = 0;
	for(int i=(3*arg->s_coll->nombre)/4;i<arg->s_coll->nombre;i++)
	{
		crypt = cryptage(*arg->M2,arg->s_coll->clef[i].K1);
		decrypt = decryptage(*arg->C2,arg->s_coll->clef[i].K2);

		if(crypt == decrypt)
		{
			arg->s_test4->nombre ++;
			arg->s_test4->clef = reallocarray(arg->s_test4->clef,arg->s_test4->nombre,sizeof(t_clef));
			arg->s_test4->clef[arg->s_test4->nombre-1].K1 = arg->s_coll->clef[i].K1;
			arg->s_test4->clef[arg->s_test4->nombre-1].K2 = arg->s_coll->clef[i].K2;
		}
	}
	printf("Thread 4 fini\n");
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

	arg.s_test1 = malloc(sizeof(solution));
	arg.s_test2 = malloc(sizeof(solution));
	arg.s_test3 = malloc(sizeof(solution));
	arg.s_test4 = malloc(sizeof(solution));
	arg.s_coll = malloc(sizeof(solution));
	arg.s_coll->clef = malloc(1*sizeof(t_clef));
	arg.s_coll->nombre = 0;
	
	return arg;
}

void freemem(donnee arg)
{
	pthread_mutex_destroy(&arg.mutex_dicho);

	free(arg.K1);
	free(arg.K2);

	free(arg.s_test1->clef);
	free(arg.s_test2->clef);
	free(arg.s_test3->clef);
	free(arg.s_test4->clef);

	free(arg.s_test1);
	free(arg.s_test2);
	free(arg.s_test3);
	free(arg.s_test4);

	free(arg.s_coll->clef);
}

int cmp(const void * a, const void * b)
{
	const message_clef* a_mc = (message_clef*)a;
	const message_clef* b_mc = (message_clef*)b;
	return ( a_mc->message - b_mc->message );
}

solution fusion(donnee arg)
{
	solution ap_test;
	int sum = arg.s_test1->nombre + arg.s_test2->nombre + arg.s_test3->nombre + arg.s_test4->nombre;
	ap_test.clef = malloc(sum*sizeof(t_clef));
	ap_test.nombre = sum;
	int y = 0;
	for(int i = 0;i<arg.s_test1->nombre;i++,y++)
	{
		ap_test.clef[y].K1 = arg.s_test1->clef[i].K1;
		ap_test.clef[y].K2 = arg.s_test1->clef[i].K2;
	}

	for(int i = 0;i<arg.s_test2->nombre;i++,y++)
	{
		ap_test.clef[y].K1 = arg.s_test2->clef[i].K1;
		ap_test.clef[y].K2 = arg.s_test2->clef[i].K2;
	}

	for(int i = 0;i<arg.s_test3->nombre;i++,y++)
	{
		ap_test.clef[y].K1 = arg.s_test3->clef[i].K1;
		ap_test.clef[y].K2 = arg.s_test3->clef[i].K2;
	}

	for(int i = 0;i<arg.s_test4->nombre;i++,y++)
	{
		ap_test.clef[y].K1 = arg.s_test4->clef[i].K1;
		ap_test.clef[y].K2 = arg.s_test4->clef[i].K2;
	}

	return ap_test;
}

solution attaque(uint32_t m1, uint32_t c1, uint32_t m2, uint32_t c2)
{
	//Calcul des clefs
	donnee arg = init_arg(&m1, &c1, &m2, &c2);
	pthread_t cryptage1;
	pthread_t cryptage2;
	pthread_t decryptage1;
	pthread_t decryptage2;

	pthread_t dichotomie1;
	pthread_t dichotomie2;

	pthread_t test1;
	pthread_t test2;
	pthread_t test3;
	pthread_t test4;
	
	int verif;
	verif = pthread_create(&cryptage1, NULL, cryptage_1, &arg);
	verif += pthread_create(&cryptage2, NULL, cryptage_2, &arg);
	verif += pthread_create(&decryptage1, NULL, decryptage_1, &arg);
	verif += pthread_create(&decryptage2, NULL, decryptage_2, &arg);
	if(verif !=0)
	{
		printf("Erreur Fatale: Un thread n'a pas pu être créer\n");
		exit(1);
	}
	verif = 0;
	pthread_join(cryptage1,NULL);
	pthread_join(cryptage2,NULL);
	pthread_join(decryptage1,NULL);
	pthread_join(decryptage2,NULL);
	printf("\n");
	//Test des clefs

	printf("deb qsort\n");
	qsort(arg.K2,16777216,sizeof(message_clef),cmp);
	printf("fin qsort\n");

	verif = pthread_create(&dichotomie1, NULL, dicho_1, &arg);
	verif += pthread_create(&dichotomie2, NULL, dicho_2, &arg);
	if(verif !=0)
	{
		printf("Erreur Fatale: Un thread n'a pas pu être créer\n");
		exit(2);
	}

	pthread_join(dichotomie1,NULL);
	pthread_join(dichotomie2,NULL);
	
	printf("\nDichotomie terminée\n");
	
	verif = pthread_create(&test1, NULL, test_1, &arg);
	verif += pthread_create(&test2, NULL, test_2, &arg);
	verif += pthread_create(&test3, NULL, test_3, &arg);
	verif += pthread_create(&test4, NULL, test_4, &arg);
	if(verif !=0)
	{
		printf("Erreur Fatale: Un thread n'a pas pu être créer\n");
		exit(3);
	}

	pthread_join(test1,NULL);
	pthread_join(test2,NULL);
	pthread_join(test3,NULL);
	pthread_join(test4,NULL);

	solution ap_test;
	ap_test = fusion(arg);
	freemem(arg);
	return ap_test;
}