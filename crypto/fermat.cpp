
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <time.h>
int64_t logtwo(int64_t  n){
	int64_t  i=0;
	while (n!=0){
		n=n>>1;
		i++;
	}
	return i-1;
}
int64_t  intsqroot(int64_t  n){
	if (n==0) return 0;
	int64_t  z = ceil((log(n)/log(2))/2);
	int64_t  x=(1<<z);
	int64_t  y=(x+n/x)/2;
	while (y<x){
		x=y;
		y=(x+n/x)/2;
	}
	return x;
}

int64_t  *Ferfactor(int64_t  n){
	int64_t  x=intsqroot(n);
	int64_t  t=2*x+1;
	int64_t  r=x*x-n;
	while (sqrt(r)!=floor(sqrt(r))||r<0){
		r=r+t;
		t=t+2;
	}
	x=(t-1)/2;
	int64_t  y=intsqroot(r);
	int64_t  *sol=(int64_t *) malloc(2);
	sol[0]=x-y;
	sol[1]=x+y;
	return sol;
}

int main(void) {
	srand(time(NULL));
	int i;
	int f=0;
	for(i=0;i<1000;i++){
		int64_t x=rand()*rand();
		x=x*(x+4);
		if (x&1){
			int64_t  *cev=Ferfactor(x);
			if (x==cev[0]*cev[1])
				printf("true\n");
			else
			{
				printf("false %llu=%llu*%llu\n",x,cev[0],cev[1]);
				f++;
			}
		}
	}
	printf("false=%d",f);
	return 0;
}

