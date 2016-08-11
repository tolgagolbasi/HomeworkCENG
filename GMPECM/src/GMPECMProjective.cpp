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

int gll(mpz_t X1, mpz_t Y1, mpz_t Z1, mpz_t X2, mpz_t Y2, mpz_t Z2,int a, mpz_t n, mpz_t res) {
	if (mpz_cmp_ui(Z1,0) == 0){
		mpz_set(X1,X2);
		mpz_set(Y1,Y2);
		mpz_set(Z1,Z2);
	}
	if (mpz_cmp_ui(Z2,0) == 0){
		return 0;
	}
	if (mpz_cmp(X1,X2)==0 && mpz_cmp(Y1,Y2)==0){
		if (mpz_cmp_ui(X1,0)==0){
			mpz_set_ui(Y1,1);
			return 0;
		}
		else{
			mpz_t delta,gamma,beta,alpha,X3,Z3,Y3,teta,teta2,p;
			mpz_init(p);
			mpz_init(delta);
			mpz_init(gamma);
			mpz_init(beta);
			mpz_init(alpha);
			mpz_init(X3);
			mpz_init(Z3);
			mpz_init(Y3);
			mpz_init(teta);
			mpz_init(teta2);
			mpz_mul(delta,Z1,Z1);
			mpz_mod(delta,delta,n);
			mpz_mul(gamma,Y1,Y1);
			mpz_mod(gamma,gamma,n);
			mpz_mul(beta,X1,gamma);
			mpz_mod(beta,beta,n);
			mpz_sub(alpha,X1,delta);
			mpz_mod(alpha,alpha,n);
			mpz_mul_ui(alpha,alpha,3);
			mpz_mod(alpha,alpha,n);
			mpz_add(teta,X1,delta);
			mpz_mod(teta,teta,n);
			mpz_mul(alpha, alpha, teta);
			mpz_mod(alpha,alpha,n);
			mpz_mul(teta,alpha,alpha);
			mpz_mod(teta,teta,n);
			mpz_mul_ui(teta2,beta,8);
			mpz_mod(teta2,teta2,n);
			mpz_sub(X3,teta,teta2);
			mpz_mod(X3,X3,n);
			mpz_set(X1,X3);
			mpz_add(teta,Y1,Z1);
			mpz_mod(teta,teta,n);
			mpz_mul(teta,teta,teta);
			mpz_mod(teta,teta,n);
			mpz_sub(teta,teta,gamma);
			mpz_mod(teta,teta,n);
			mpz_sub(Z3,teta,delta);
			mpz_mod(Z1,Z3,n);
			mpz_mul_ui(teta,beta,4);
			mpz_mod(teta,teta,n);
			mpz_sub(teta,teta,X3);
			mpz_mod(teta,teta,n);
			mpz_mul(teta,teta,alpha);
			mpz_mod(teta,teta,n);
			mpz_mul(teta2,gamma,gamma);
			mpz_mod(teta2,teta2,n);
			mpz_mul_ui(teta2,teta2,8);
			mpz_mod(teta2,teta2,n);
			mpz_sub(Y1,teta,teta2);
			mpz_mod(Y1,Y3,n);
			mpz_clear(delta);
			mpz_clear(gamma);
			mpz_clear(beta);
			mpz_clear(alpha);
			mpz_clear(X3);
			mpz_clear(Z3);
			mpz_clear(Y3);
			mpz_gcd(p,Z1,n);
			if(mpz_cmp_ui(p,1)!=0)
				mpz_set(res,p);
			mpz_clear(teta);
			mpz_clear(teta2);
			return 0;
		}
	}
	else{
		mpz_t Z1Z1,Z2Z2,U1,U2,S1,S2,H,I,J,r,V,r2,X3,Y3,Z3,p;
		mpz_init(p);
		mpz_init(Z1Z1);
		mpz_init(Z2Z2);
		mpz_init(U1);
		mpz_init(U2);
		mpz_init(S1);
		mpz_init(S2);
		mpz_init(H);
		mpz_init(I);
		mpz_init(J);
		mpz_init(r);
		mpz_init(V);
		mpz_init(X3);
		mpz_init(Y3);
		mpz_init(Z3);
		mpz_init(r2);
		mpz_mul(Z1Z1,Z1,Z1);
		mpz_mod(Z1Z1,Z1Z1,n);
		mpz_mul(Z2Z2, Z2,Z2);
		mpz_mod(Z2Z2,Z2Z2,n);
		mpz_mul(U1, X1,Z2Z2);
		mpz_mod(U1,U1,n);
		mpz_mul(U2, X2,Z1Z1);
		mpz_mod(U2,U2,n);
		mpz_mul(S1, Y1,Z2);
		mpz_mod(S1,S1,n);
		mpz_mul(S1,S1,Z2Z2);
		mpz_mod(S1,S1,n);
		mpz_mul(S2, Y2,Z1);
		mpz_mod(S2,S2,n);
		mpz_mul(S2,S2,Z1Z1);
		mpz_mod(S2,S2,n);
		mpz_sub(H, U2,U1);
		mpz_mod(H,H,n);
		mpz_mul_ui(I,H,2);
		mpz_mod(I,I,n);
		mpz_mul(I,I,I);
		mpz_mod(I,I,n);
		mpz_mul(J, H,I);
		mpz_mod(J,J,n);
		mpz_sub(r,S2,S1);
		mpz_mod(r,r,n);
		mpz_mul_ui(r,r,2);
		mpz_mod(r,r,n);
		mpz_mul(V, U1,I);
		mpz_mod(V,V,n);
		mpz_mul_ui(V,V,2);
		mpz_mod(V,V,n);
		mpz_sub(J,J,V);
		mpz_mod(J,J,n);
		mpz_sub(X3,r2,J);
		mpz_mod(X3,X3,n);
		mpz_set(X1,X3);
		mpz_mul(S1,J,S1);
		mpz_mod(S1,S1,n);
		mpz_mul_ui(S1,S1,2);
		mpz_mod(S1,S1,n);
		mpz_sub(V,V,X3);
		mpz_mod(V,V,n);
		mpz_sub(r,V,r);
		mpz_mod(r,r,n);
		mpz_sub(Y3,r,S1);
		mpz_mod(Y1,Y3,n);
		mpz_add(Z1,Z2,Z1);
		mpz_mod(Z1,Z1,n);
		mpz_mul(Z1,Z1,Z1);
		mpz_mod(Z1,Z1,n);
		mpz_sub(Z1,Z1,Z1Z1);
		mpz_mod(Z1,Z1,n);
		mpz_sub(Z1,Z1,Z2Z2);
		mpz_mod(Z1,Z1,n);
		mpz_mul(Z3,Z1,H);
		mpz_mod(Z3,Z3,n);
		mpz_set(Z1,Z3);
		mpz_gcd(p,Z1,n);
		if(mpz_cmp_ui(p,1)!=0)
			mpz_set(res,p);
		mpz_clear(Z1Z1);
		mpz_clear(Z2Z2);
		mpz_clear(U1);
		mpz_clear(U2);
		mpz_clear(S1);
		mpz_clear(S2);
		mpz_clear(H);
		mpz_clear(I);
		mpz_clear(J);
		mpz_clear(r);
		mpz_clear(V);
		mpz_clear(X3);
		mpz_clear(Y3);
		mpz_clear(Z3);
		mpz_clear(r2);
		return 0;
	}
}
void mult2l(int b1, mpz_t* P, int a, mpz_t n) {
	mpz_t R[3];
	mpz_t res;
	mpz_init(res);
	mpz_init_set_ui(R[0],0);
	mpz_init_set_ui(R[1],1);
	mpz_init_set_ui(R[2],0);
	int b2,b;
	for (b2 = 2; b2 < b1; b2++){
		b = b2;
		while (b != 0){
			if ((b % 2) == 1){
				gll(P[0], P[1], P[2], R[0], R[1], R[2], a, n,res);
				if (mpz_cmp_ui(res,1)!=0){
					mpz_set(P[0],res);
					mpz_clear(R[0]);
					mpz_clear(R[1]);
					mpz_clear(R[2]);
					return;
				}
			}
			b = b / 2;
			gll(P[0], P[1], P[2], P[0], P[1], P[2], a, n,res);
			if (mpz_cmp_ui(res,1)!=0){
				mpz_set(P[0],res);
				mpz_clear(R[0]);
				mpz_clear(R[1]);
				mpz_clear(R[2]);
				return;
			}
		}
	}
	mpz_clear(R[0]);
	mpz_clear(R[1]);
	mpz_clear(R[2]);
}
int ecm(mpz_t z,mpz_t n, int bounda, int boundb) {
	unsigned long y2;
	unsigned long Bp = 1000;
	int a,b;
	unsigned long x;
	mpz_t fpoint[3];
	for (a = 1; a <= bounda; a++){
		if ((4*a*a*a + 27) == 0)
			continue;
		for (b = 2; b <= boundb; b++){
			x = 0;
			do{
				x = x + 1;
				y2 = x*x*x + (a*x) + b;
			} while (!IsSquare(y2)&& x<100);
			if (x>=100){ continue; }
			mpz_init_set_ui(fpoint[0],x);
			mpz_init_set_ui(fpoint[1],Isqrt(y2));
			mpz_init_set_ui(fpoint[2],1);
			mult2l(Bp, fpoint, a, n);
			if (mpz_cmp_ui(fpoint[2],0)==0)
				break;
			mpz_init(z);
			mpz_gcd(z, fpoint[0], n);
			if (mpz_cmp_ui(z,1)!=0){
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
		n[0] = rand()/100;
		n[1] = rand();
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
//		mpz_init_set_ui(z,3377691921204847);
		ecm(factor,z, 100, 100);
		mpz_out_str(NULL,10,z);
		printf(" mod ");
		mpz_out_str(NULL,10,factor);
		printf(";\n");
	}
}
