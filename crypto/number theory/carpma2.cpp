#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
int myGlobal = 0;
unsigned long long int* top(int s,unsigned long long int *xe,unsigned long long int *ye)
{
	int i;
	unsigned long long int temp=0;
	unsigned long long int* sonuc;
	sonuc = (unsigned long long int*)malloc( s*sizeof( unsigned long long int ));
	for(i=0;i<s;++i)
	{
		sonuc[i]=0;
	}
	for(i=0;i<s;++i)
	{
		temp=xe[i]+ye[i]+sonuc[i];
		if (temp<xe[i])
		{
			++sonuc[i+1];
			if(i+1==s)
				myGlobal=1;

		}
		sonuc[i]=temp;
	}
	return sonuc;
}
unsigned long long int* carp(int s,unsigned long long int *xe,unsigned long long int *ye)
{
	int i,j;
	unsigned long long int* sonuc;
	sonuc = (unsigned long long int*)malloc( s*2*sizeof( unsigned long long int ));
	if (s>1)
	{
		unsigned long long int *firstHalfx = xe;
		unsigned long long int *secondHalfx = &xe[s/2];
		unsigned long long int *firstHalfy = ye;
		unsigned long long int *secondHalfy = &ye[s/2];
		unsigned long long int* sonuc1[4];
		sonuc1[0]=(unsigned long long int*)malloc( s/2*sizeof( unsigned long long int ));
		sonuc1[1]=(unsigned long long int*)malloc( s/2*sizeof( unsigned long long int ));
		sonuc1[2]=(unsigned long long int*)malloc( s/2*sizeof( unsigned long long int ));
		sonuc1[3]=(unsigned long long int*)malloc( s/2*sizeof( unsigned long long int ));
		unsigned long long int* x1;
		unsigned long long int* x2[2];
		unsigned long long int* y2[2];
		unsigned long long int *right,*left;
		x2[0]=firstHalfx;
		x2[1]=secondHalfx;
		y2[0]=firstHalfy;
		y2[1]=secondHalfy;
		x1 = carp(s/2,y2[0],x2[0]);
		right=x1;
		left=x1+(s/2);
		myGlobal=0;
		memcpy(sonuc1[0],right,(s/2)*sizeof( unsigned long long int ));
		memcpy(sonuc1[1],left,(s/2)*sizeof( unsigned long long int ));
		x1 = carp(s/2,y2[1],x2[0]);
		right=x1;
		left=x1+ s/2;
		memcpy(sonuc1[1],top(s/2,sonuc1[1],right),(s/2)*sizeof( unsigned long long int ));
		left[0]+=myGlobal;
		myGlobal=0;
		memcpy(sonuc1[2],left,(s/2)*sizeof( unsigned long long int ));
		x1 = carp(s/2,y2[0],x2[1]);
		right=x1;
		left=x1+ s/2;
		memcpy(sonuc1[1],top(s/2,sonuc1[1],right),(s/2)*sizeof( unsigned long long int ));
		left[0]+=myGlobal;
		myGlobal=0;
		memcpy(sonuc1[2],top(s/2,sonuc1[2],left),(s/2)*sizeof( unsigned long long int ));
		x1 = carp(s/2,y2[1],x2[1]);
		right=x1;
		left=x1+ s/2;
		right[0]+=myGlobal;
		myGlobal=0;
		memcpy(sonuc1[2],top(s/2,sonuc1[2],right),(s/2)*sizeof( unsigned long long int ));
		left[0]+=myGlobal;
		myGlobal=0;
		memcpy(sonuc1[3],left,(s/2)*sizeof( unsigned long long int ));
		for(i=0;i<4;++i)
		{
			for(j=0;j<s;++j)
			{
				memcpy(sonuc+i*(s/2)+j,sonuc1[i]+j,(s/2)*sizeof( unsigned long long int ));
			}
		}
	return sonuc;
	}
	else
	{
		unsigned long long int sakla;
		unsigned long int sonuc32[4]={0};
		unsigned long long int x1 =1;
		unsigned long int x2[2];
		unsigned long int y2[2];
		unsigned long int right,left;
		x2[0]=(xe[0]<<32)>>32;
		x2[1]=xe[0]>>32;
		y2[0]=(ye[0]<<32)>>32;
		y2[1]=ye[0]>>32;
		for(i=0;i<2;++i)
		{
			for(j=0;j<2;++j)
			{
				x1 = x1*y2[i]*x2[j];
				right=(x1<<32)>>32;
				left=x1>>32;
	   			sakla=sonuc32[j+i]+right;
	   			if (sakla<sonuc32[j+i])
	   			{
					sonuc32[j+i]=sakla;
					sonuc32[j+i+1]++;
				}
				else sonuc32[j+i]=sakla;
	  			sakla=sonuc32[j+i+1]+left;
	   			if (sakla<sonuc32[j+i+1])
	   			{
					sonuc32[j+i+1]=sakla;
					sonuc32[j+i+2]++;
				}
				else sonuc32[j+i+1]=sakla;
	   			x1 =1;
			}
		}
		unsigned long long int f=pow(2,32);
		sonuc[0]=sonuc32[1]*f+sonuc32[0];
		sonuc[1]=sonuc32[3]*f+sonuc32[2];
		return sonuc;
	}
}
int main()
{
srand(time(NULL));// random number from time
	int bx=2;
	int by=2;
	int i;
	unsigned long long int x[bx];
	unsigned long long int y[by];
	for(i=bx-1;i>=0;--i)
	{
		x[i]=rand()*rand()*10000;
		printf("%I64u ",x[i]);
	}
	printf("\n");
	for(i=bx-1;i>=0;--i)
	{
		y[i]=rand()*rand()*10000;
		printf("%I64u ",y[i]);
	}
	printf("\n");
	unsigned long long int *sonuc;
	sonuc=(unsigned long long int*)malloc( bx*2*sizeof( unsigned long long int ));
	memcpy(sonuc,carp(bx,x,y),(bx*2)*sizeof( unsigned long long int ));
	for(i=bx+by-1;i>=0;i--){
		printf("%I64u ",sonuc[i]);
	}
	return 1;
}

