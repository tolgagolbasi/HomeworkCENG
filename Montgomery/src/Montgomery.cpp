#include <stdio.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <stdint.h>
#include <gmp.h>
#include <stdlib.h>
using namespace std;
long Isqrt(long x){
	long   squaredbit, remainder, root;

	if (x < 1) return 0;
	squaredbit = (long)((((unsigned long)~0L) >> 1) &
		~(((unsigned long)~0L) >> 2));
	remainder = x;  root = 0;
	while (squaredbit > 0) {
		if (remainder >= (squaredbit | root)) {
			remainder -= (squaredbit | root);
			root >>= 1; root |= squaredbit;
		}
		else {
			root >>= 1;
		}
		squaredbit >>= 2;
	}

	return root;
}
bool IsSquare(long n)
{
	if (n < 0)
		return false;

	long tst = (long)(sqrt(n) + 0.5);
	return (tst*tst == n);
}

int gll(mpz_t* X, mpz_t* Y, int a, mpz_t n) {
	if (mpz_cmp_ui(X[1],0) == 0){
		for (int i = 0; i < 2; i++)
			mpz_set(X[i],Y[i]);
		return 1;
	}
	if (mpz_cmp_ui(Y[1],0) == 0){
		return 1;
	}
	if (mpz_cmp(X[0],Y[0])==0){
		mpz_t t0,t9,t10,t11,t1,t2,t3,t4,t5,t6,t7,t8;
		mpz_init_set_ui(t0,2);
		mpz_init_set_ui(t9,3);
		mpz_init_set_ui(t10,a);
		mpz_init(t11);
		mpz_init(t1);
		mpz_init(t2);
		mpz_init(t3);
		mpz_init(t4);
		mpz_init(t5);
		mpz_init(t6);
		mpz_init(t7);
		mpz_init(t8);
		mpz_mul(t1,t0,X[0]);
		mpz_mod(t1,t1,n);
		mpz_mul(t2,X[0],X[0]);
		mpz_mod(t2,t2,n);
		mpz_mul(t3,t9,t2);
		mpz_mod(t3,t3,n);
		mpz_gcd(t11, n, t1);
		if (mpz_cmp_ui(t11,1)!=0){
			mpz_set(X[2],t1);
			return 1;
		}
		mpz_add(t4,t10,t3);
		mpz_invert(t5,t1,n);
		mpz_mul(t6,t4,t5);
		mpz_mod(t6,t6,n);
		mpz_mul(t7,t6,t6);
		mpz_mod(t7,t7,n);
		mpz_t x;
		mpz_init(x);
		mpz_sub(x,t7,t1);
		mpz_sub(t8,X[0],x);
		mpz_mul(t9,t8,t6);
		mpz_mod(t9,t9,n);
		mpz_t y;
		mpz_init(y);
		mpz_sub(y,t9,X[1]);
		mpz_set(X[0],x);
		mpz_set(X[1],y);
		mpz_set_ui(X[2],1);
		mpz_clear(x);
		mpz_clear(y);
		mpz_clear(t0);
		mpz_clear(t9);
		mpz_clear(t10);
		mpz_clear(t11);
		mpz_clear(t1);
		mpz_clear(t2);
		mpz_clear(t3);
		mpz_clear(t4);
		mpz_clear(t5);
		mpz_clear(t6);
		mpz_clear(t7);
		mpz_clear(t8);
		return 1;
	}
	else{
		mpz_t t0,t9,t10,t11,t1,t2,t3,t4,t5,t6,t7,t8;
		mpz_init(t0);
		mpz_init(t10);
		mpz_init(t11);
		mpz_init(t1);
		mpz_init(t2);
		mpz_init(t3);
		mpz_init(t4);
		mpz_init(t5);
		mpz_init(t6);
		mpz_init(t7);
		mpz_init(t8);
		mpz_init(t9);
		mpz_sub(t1,Y[0],X[0]);
		mpz_gcd(t9, t1, n);
		if (mpz_cmp_ui(t9,1)!=0){
			mpz_set(X[2],t1);
			return 1;
		}
		mpz_sub(t2,X[1],Y[1]);
		mpz_sub(t3,X[0],Y[0]);
		mpz_invert(t4,t3,n);
		mpz_mul(t5,t2,t4);
		mpz_mod(t5,t5,n);
		mpz_mul(t6,t5,t5);
		mpz_mod(t6,t6,n);
		mpz_t x;
		mpz_init(x);
		mpz_sub(x, t6, t1);
		mpz_mod(x,x,n);
		mpz_sub(t7, X[0], x);
		mpz_mod(t7,t7,n);
		mpz_mul(t8,t5,t7);
		mpz_mod(t8,t3,n);
		mpz_t y;
		mpz_init(y);
		mpz_sub(y,t8,X[1]);
		mpz_mod(y,y,n);
		mpz_set(X[0],x);
		mpz_set(X[1],y);
		mpz_set_ui(X[2],1);
		mpz_clear(x);
		mpz_clear(y);
		mpz_clear(t0);
		mpz_clear(t9);
		mpz_clear(t10);
		mpz_clear(t11);
		mpz_clear(t1);
		mpz_clear(t2);
		mpz_clear(t3);
		mpz_clear(t4);
		mpz_clear(t5);
		mpz_clear(t6);
		mpz_clear(t7);
		mpz_clear(t8);
		return 1;
	}

}
void mult2l(int b1, mpz_t* X, int a, mpz_t n) {
	mpz_t r[2];
	mpz_init_set_ui(r[0],0);
	mpz_init_set_ui(r[1],1);
	int b2,b;
	for (b2 = 2; b2 < b1; b2++){
		b = b2;
		while (b != 0){
			if ((b % 2) == 1){
				gll(X, r, a, n);
			}
			b = b / 2;
			gll(X, X, a, n);
			if (mpz_cmp_ui(X[2],1)>0)
				return;
		}
	}
	mpz_clear(r[0]);
	mpz_clear(r[1]);
	mpz_clear(r[2]);
}
int ecm(mpz_t z,mpz_t n, int bounda, int boundb) {
	unsigned long y2;
	unsigned long Bp = 100;
	int a,b;
	unsigned long x;
	mpz_t fpoint[2];
	for (a = 1; a <= bounda; a++){
		if ((4*a*a*a + 27) == 0)
			continue;
		for (b = 2; b <= boundb; b++){
			x = 0;
			do{
				x = x + 1;
				if(((x*x*x + (a*x*x) + x)%b) == 0)
					y2 = (x*x*x + (a*x*x) + x)/b;
			} while (!IsSquare(y2)&& x<100);
			if (x>=100){ continue; }
			mpz_init_set_ui(fpoint[0],(x / z));
			mpz_init_set_ui(fpoint[1],Isqrt(y2)/z);
			mult2l(Bp, fpoint, a, n);
			if (mpz_cmp_ui(fpoint[1],0)==0)
				break;
			mpz_init(z);
			mpz_gcd(z, fpoint[2], n);
			if (mpz_cmp_ui(z,1)>0){
				printf("a:%d b:%d x:%d\n",a,b,x);
				return 1;
			}
		}
	}
	return 0;
}
int main() {
	srand((unsigned)time(NULL));
	unsigned long n[2];
	for (int i = 0; i < 10; i++){
		n[0] = rand()/10000;
		n[1] = rand()*rand();
		mpz_t x;
		mpz_t z1,z2,z;
		mpz_init(x);
		mpz_init(z);
		mpz_init(z1);
		mpz_init(z2);
		mpz_import(x, 1, -1, sizeof(n[0]), 0, 0, n);
		mpz_nextprime(z1, x);
		mpz_import(x, 1, -1, sizeof(n[1]), 0, 0, n+1);
		mpz_nextprime(z2, x);
		mpz_mul(z,z1,z2);
		mpz_t factor;
		mpz_init(factor);
//		mpz_init_set_ui(z,8774208478331381);
		ecm(factor,z, 100, 100);
		mpz_out_str(NULL,10,z);
		printf(" mod ");
		mpz_out_str(NULL,10,factor);
		printf(";\n");

	}
}
