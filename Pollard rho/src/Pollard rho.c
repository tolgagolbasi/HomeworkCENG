#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
uint64_t func(uint64_t x,uint64_t b,uint64_t n){
	return (x*x+b)%n;
}
uint64_t gcd(uint64_t a,uint64_t b){
	if (b==0)
		return a;
	else
		return gcd(b, a%b);
}
uint64_t pollard(uint64_t n){
	srand(time(NULL));
	uint64_t b = rand();
	b <<= 32;
	b += rand();
	b = (b%(n-3))+1;
	uint64_t s = rand();
	s <<= 32;
	s += rand();
	s = (s%(n-1));
	uint64_t g = 1;
	uint64_t A,B=s;
	while(g==1){
		A=func(A,b,n);
		B=func(func(B,b,n),b,n);
		g=gcd(A-B,n);
	}
	if (g < n)
		return g;
	else
		return pollard(n);

}
int main(void) {
	srand(time(NULL));
	uint64_t x = (uint64_t)rand()*rand()*rand()*rand();
	uint64_t a=pollard(x);
	printf("%llu divides %llu",a,x);
	return 0;
}
