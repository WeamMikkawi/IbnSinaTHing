#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <gmp.h> 
#include <time.h>
mpz_t p;
mpz_t q;

mpz_t pPrime;
int main()
{
	mpz_init(pPrime);
	mpz_init_set_str(p,"31",10);
	mpz_init_set_str(q,"101",10);

	mpz_gcd(pPrime,p,q);
	gmp_printf("%Zd\n",pPrime);
	return 0;

}
