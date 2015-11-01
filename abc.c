#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <gmp.h> 
#include <time.h>


#define initNbLength 2

void generateInitNb(char * initNb, int len);
void keysGeneration();


// Add those to paillier.h when u create it 	
mpz_t p;
mpz_t q;
mpz_t n;
mpz_t lambda;
mpz_t g;
mpz_t nn; // n squared 
mpz_t initNbMpz;
mpz_t gcdResult;
mpz_t lcmResult;
mpz_t tmpI;
mpz_t tmpII;
mpz_t p_1;
mpz_t q_1;
time_t t;
/*
Generate Private Public Keys
*/
void keysGeneration(){
	//char* initNbChar = malloc(sizeof(char) * (initNbLength+1));
	char* initNbChar;
	mpz_init(gcdResult);
	do{
		 initNbChar = malloc(sizeof(char) * (initNbLength+1));
		generateInitNb(initNbChar, initNbLength);
		mpz_set_str(initNbMpz, initNbChar, 10/*decimal*/);
		gmp_printf("it is a start :3 :%Zd\n",initNbMpz); //$$$$$$$$$$$$$$$$$
		mpz_nextprime (p, initNbMpz);
		gmp_printf("next-prime > :%Zd\n",p); //$$$$$$$$$$$$$$$$$$$$$
		 initNbChar = malloc(sizeof(char) * (initNbLength+1));
		 generateInitNb(initNbChar, initNbLength);
		 mpz_set_str(initNbMpz, initNbChar, 10/*decimal*/);
		mpz_nextprime (q, initNbMpz);
		gmp_printf("next-prime-q :%Zd\n",q); //%^#^@%^@%

		mpz_sub_ui(p_1, p, 1);
		gmp_printf("next-prime > :%Zd\n",p_1);
		mpz_sub_ui(q_1, q, 1);
		gmp_printf("next-prime > :%Zd\n",q_1);

		mpz_mul(n, p, q);
		gmp_printf("next-prime > :%Zd\n",n);
		
		mpz_mul(tmpI, p_1, q_1);
		gmp_printf("next-prime > :%Zd\n",tmpI);
		mpz_gcd(gcdResult, tmpI, n);
		gmp_printf("next-prime > :%Zd\n",gcdResult);
		return;
		
	}while(mpz_cmp_ui(gcdResult, 1) != 0); // check if (gcd(pq, (p-1)(q-1)) = 1
//	mpz_mul(n, p, q);
	mpz_mul(nn, n, n);
	mpz_lcm(lambda, p_1, q_1);

	// generate a number g within the range 0 to n*n-1 inclusive ! 
	// this way i can get rid of using mod n^2 on this step 
	gmp_randstate_t state;
	gmp_randinit_mt (state); // initiallize state with Mersenne Twister which is basically fast one !
	mpz_urandomm(g, state, nn);
	// Now we check if g is good enough for us 
	mpz_powm(tmpI, g/*base*/, lambda/*exp*/, nn/*mod*/); // tmp = base^exp modulo mod
	// now L(u) = (u-1)/n
	mpz_sub_ui(tmpI, tmpI, 1);
	mpz_tdiv_q(tmpI, tmpI, n);

	mpz_gcd(gcdResult, n, tmpI);
	if (mpz_cmp_ui(gcdResult, 1) != 0){
		printf("G is not proper !!!\n");
		exit(EXIT_FAILURE);
	}


	printf("Keys Generation Result : \n");
	printf("Start -- \n");
	printf("Public Key  : \n");
	gmp_printf ("\nn = %Zd\n", n);
	gmp_printf ("\ng= %Zd\n", g);
	printf("\nPrivate Key  : \n");
	gmp_printf ("\nlambda = %Zd\n", lambda);
	printf("Micro :3 \n");
	printf("End --  \n");
	
	
}
/*
Do the Encryption over the data
*/
mpz_t r;
mpz_t c;
void encrypt(int msg/*To be extended to other types !*/){
	// generate r between 0 and n-1 inclusive 
	unsigned long int m = msg;
	gmp_randstate_t state;
	gmp_randinit_mt (state); // initiallize state with Mersenne Twister which is basically fast one !
	mpz_urandomm(r, state, n);

	mpz_powm_ui(tmpI, g/*base*/, m/*exp*/, nn/*mod*/); // tmp = base^exp modulo mod
	mpz_powm(tmpII, r/*base*/, n/*exp*/, nn/*mod*/); // tmp = base^exp modulo mod
	// Remember : a.b mod n = ((a mod n).(b mod n)) mod n
	mpz_mul(c, tmpI, tmpII);
	mpz_mod(c, c, nn);

	gmp_printf ("\nc = %Zd\n", c);
}

void decryption(){
	
	mpz_powm(tmpI, c/*base*/, lambda/*exp*/, nn/*mod*/); // tmp = base^exp modulo mod
	mpz_powm(tmpII, g/*base*/, lambda/*exp*/, nn/*mod*/); // tmp = base^exp modulo mod
	mpz_tdiv_q (tmpI, tmpI, tmpII);  
	mpz_mod(tmpI, tmpI, n);

	gmp_printf ("\nDecryption = %Zd\n", tmpI);
}
/*
Make the Decryption
*/

/*
This function is to generate a XXXish long random character sequence 
*/

void generateInitNb(char * initNb, int len){
    
	int i;
	int max = 57; //assci for 9
	int min = 48; // assci for 0
	for(i = 0; i < len; i++){
		int j = rand() % (max - min + 1) + min;
		initNb[i] = (char)j;
	}
	initNb[i] = '\0';
	//printf("\nFrom Generation = \n%s\n", initNb);
}
int main(){

	//[Declaration]//mpz_t  r; // this is how to declare int in gmp
	//[initiallization]// mpz_init (r); // this is how i initialize the variables 
	//[Print]//gmp_printf ("%Zd\n", r); // this is how i print'em 
	//[set x = string]//mpz_set_str : https://gmplib.org/manual/Assigning-Integers.html#Assigning-Integers
	//[*]//void mpz_mul (mpz_t rop, const mpz_t op1, const mpz_t op2) 
	//[Quotient]//void mpz_cdiv_q (mpz_t q, const mpz_t n, const mpz_t d)
	//[Reminder]//void mpz_cdiv_r (mpz_t r, const mpz_t n, const mpz_t d)
	//[Exponentiation] //void mpz_powm (mpz_t rop, const mpz_t base, const mpz_t exp, const mpz_t mod)

	//[GCD]void mpz_gcd (mpz_t rop, const mpz_t op1, const mpz_t op2)
	//void mpz_lcm (mpz_t rop, const mpz_t op1, const mpz_t op2)

	//void mpz_nextprime (mpz_t rop, const mpz_t op)
	// int mpz_probab_prime_p (const mpz_t n, int reps) // which uses Miller-Rabin probabilistic primality tests. 
	
	
	//printf("%s\n", initNb);
	
	

	/*****************PROMBLEM**************************/
	/*No matter how many times I apply the algo it seems to be not sure if it's Prime */
	/**********************************/
	       /******************/
	           /**********/
	             /*****/
	              /**/ 
	
	
	srand((unsigned) time(&t));
	keysGeneration();
	return 0;
	encrypt(6000);
	decryption();

	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(n);
	mpz_clear(nn);
	mpz_clear(lambda);
	mpz_clear(g);
	mpz_clear(initNbMpz);
	mpz_clear(p_1);
	mpz_clear(q_1);
	mpz_clear(tmpI);
	mpz_clear(gcdResult);
	mpz_clear(lcmResult);
	mpz_clear(r);
	mpz_clear(c);

	return 0;
}