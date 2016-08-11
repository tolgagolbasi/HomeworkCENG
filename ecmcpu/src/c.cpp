//============================================================================
// Name        : Elliptic.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
using namespace std;
uint32_t ipow(uint32_t base, uint32_t exp)
{
	uint32_t result = 1;
	while (exp)
	{
		if (exp & 1)
			result *= base;
		exp >>= 1;
		base *= base;
	}

	return result;
}
uint32_t mod_inv(uint32_t a, uint32_t b)
{
	uint32_t b0 = b, t, q;
	uint32_t x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1) {
		q = a / b;
		t = b, b = a % b, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) x1 += b0;
	return x1;
}
long Isqrt(long x){
	long   squaredbit, remainder, root;

	if (x < 1) return 0;

	/* Load the binary constant 01 00 00 ... 00, where the number
	* of zero bits to the right of the single one bit
	* is even, and the one bit is as far left as is consistant
	* with that condition.)
	*/
	squaredbit = (long)((((unsigned long)~0L) >> 1) &
		~(((unsigned long)~0L) >> 2));
	/* This portable load replaces the loop that used to be
	* here, and was donated by  legalize@xmission.com
	*/

	/* Form bits of the answer. */
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
uint32_t gcd(uint32_t a, uint32_t b) {
	uint32_t c;
	while (a != 0) {
		c = a;
		a = b%a;
		b = c;
	}
	return b;
}
uint32_t lcm(uint32_t B) {
	uint32_t a = 2;
	for (uint32_t i = 3; i <= B; i++)
		a = a * i / gcd(a, i);
	return a;
}

int gll(uint32_t P[], uint32_t Q[], uint32_t a, uint32_t n, uint32_t *result) {
	if (P[2] == 0){
		for (uint32_t i = 0; i < 3; i++)
			result[i] = Q[i];
		return 1;
	}
	if (0 == Q[2]){
		for (int i = 0; i < 3; i++)
			result[i] = P[i];
		return 1;
	}
	if (P[0] == Q[0]){
		if (P[1] != Q[1]){
			result[1] = 1;
			return 1;
		}
		if (P[0] == 0){
			result[1] = 1;
			return 1;
		}
		else{
			if (gcd(2 * P[0], n) != 1){
				result[2] = 2 * P[0];
				return 1;
			}
			uint32_t x = (ipow(((3 * ipow(P[0], 2)) + a)*(mod_inv(2 * P[0], n)), 2) - 2 * P[0]) % n;
			uint32_t y = ((((3 * ipow(P[0], 2)) + a)*mod_inv(2 * P[0], n))*(P[0] - x) - P[1]) % n;
			result[0] = x;
			result[1] = y;
			result[2] = 1;
			return 1;
		}
	}
	else{
		if (gcd(Q[0] - P[0], n) != 1){
			result[2] = Q[0] - P[0];
			return 1;
		}
		uint32_t x = (ipow((P[1] - Q[1])*(mod_inv(P[0] - Q[0], n)), 2) - P[0] - Q[0]) % n;
		uint32_t y = (((P[1] - Q[1])*mod_inv(P[0] - Q[0], n))*(P[0] - x) - P[1]) % n;
		result[0] = x;
		result[1] = y;
		result[2] = 1;
		return 1;
	}

}

void mult2l(uint32_t b1, uint32_t P[], uint32_t a, uint32_t n) {
	uint32_t ans[3] = { 0, 0, 0 };
	uint32_t result[3] = { 0, 1, 0 };
	uint32_t pow2P[3];
	for (uint32_t i = 0; i < 3; i++){
		pow2P[i] = P[i];
	}
	for (uint32_t b = 2; b < b1; b++){
		while (b != 0){
			if ((b % 2) == 1){
				gll(pow2P, result, a, n, ans);
				pow2P[0] = ans[0];
				pow2P[1] = ans[2];
				pow2P[2] = ans[3];
			}
			b = b / 2;
			gll(pow2P, pow2P, a, n, ans);
			pow2P[0] = ans[0];
			pow2P[1] = ans[1];
			pow2P[2] = ans[2];
			if ((pow2P[2] > 1) || (pow2P[2] == 0))
				break;
		}
		if ((pow2P[2] > 1) || (pow2P[2] == 0))
			break;
	}
	for (uint32_t i = 0; i < 3; i++){
		P[i] = pow2P[i];
	}

}
uint32_t ecm(uint32_t n, uint32_t bounda, uint32_t boundb) {
	uint32_t y2;
	uint32_t Bp = 100;
	for (uint32_t a = 1; a <= bounda; a++){
		if (((4 * ipow(a, 3) + 27) % n) == 0)
			continue;
		for (uint32_t b = 1; b <= boundb; b++){
			uint32_t x = 0;
			do{
				y2 = (x*x*x + (a*x) + b) % n;
				//printf("%d\n",y2);
				x = x + 1;
			} while (!IsSquare(y2)&& x<50);//mod n olmas� laz�m
			if (x>=50){ continue; }
			uint32_t fpoint[] = { x, Isqrt(y2), 1 };
			//printf("(%d,",fpoint[0]);
			//printf("%d,",fpoint[1]);
			//printf("%d)\n",fpoint[2]);
			mult2l(Bp, fpoint, a, n);
			//printf("(%d,",fpoint[0]);
			//printf("%d,",fpoint[1]);
			//printf("%d)\n",fpoint[2]);
			if (fpoint[2] == 0)
				break;
			uint32_t g = gcd(fpoint[2], n);
			if (g > 1){
			//	if (g!=2)
			//		printf("g=%d\n", g);
			//	printf("a=%d b=%d\n", a,b);
				return g;
			}
		}
	}
	printf("Factorization(%d);\n", n);
	return 0;
}
int main() {
	char str[80];
	srand((unsigned)time(NULL));
	uint32_t n;
	for (uint32_t i = 0; i < 256; i++){
		n = rand() | (rand()) << 16;
		printf("%d\n", ecm(n, 5, 5));
	}
	scanf("%s", str);
}
