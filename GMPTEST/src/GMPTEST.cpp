//============================================================================
// Name        : GMPTEST.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <gmp.h>
#include <stdio.h>
using namespace std;

int main() {
	unsigned long t[2]={7586786,7867861};
	char inputx[1024]="753753753732732727775252";
	char inputy[1024]="432";
	int flag;
	mpz_t x;
	mpz_t y;
	mpz_t z;
	mpz_init (x);
	mpz_init (y);
	mpz_init (z);
	mpz_set_ui(z,0);
	mpz_import (x, 2, -1, sizeof(t[0]), 0, 0, t);

	flag = mpz_set_str(y,inputy, 10);
	mpz_gcd(z,x,y);
	mpz_out_str(NULL,10,z);
	return 0;
}
