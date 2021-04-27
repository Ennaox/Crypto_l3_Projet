#include "decryptage.c"
#include <pthread.h>

struct donnee{
	uint32_t *M1;
	uint32_t *C1;

	uint32_t *M2
	uint32_t *C2;

	uint32_t* K1;
	uint32_t* K2;

	uint32_t* state_k1;
	uint32_t* state_k2;

	uint16_t* end_bool;
}