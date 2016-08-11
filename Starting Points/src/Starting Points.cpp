//============================================================================
// Name        : Starting.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <math.h>
using namespace std;
bool IsSquare(long n)
{
	if (n < 0)
		return false;

	long tst = (long)(sqrt(n) + 0.5);
	return (tst*tst == n);
}
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
int main() {
	int a,b;
	unsigned long x,y2;
	for (a = 1; a <= 100; a++){
		if ((4*a*a*a + 27) == 0)
			continue;
		for (b = 2; b <= 100; b++){
			x = 0;
			do{
				x = x + 1;
				y2 = x*x*x + (a*x) + b;
			} while (!IsSquare(y2)&&x<100);
			if (x>=100){ continue; }
			printf("E={%d,%d} P={%lu,%lu}\n",a,b,x,Isqrt(y2));
		}
	}
}
