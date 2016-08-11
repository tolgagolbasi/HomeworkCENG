#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
int main()
{	
	int radix=10;
    unsigned long nu1[5] = {4,2,2,2,2};
    unsigned long nu2[5] = {4,2,2,2,2};
	unsigned long output[9] = {0};
	unsigned long l1 = nu1[0];
	unsigned long l2 = nu2[0];
	output[0] = l1 + l2;
    unsigned long stepm = l1; 
    while( stepm > 0 ){
        unsigned long stepn = l2;
        unsigned long carry = 0;
        while( stepn > 0 ){
            unsigned long part = nu1[stepm] * nu2[stepn] + output[stepm + stepn] + carry;
            output[stepm + stepn] = part % radix;
            carry = part / radix;
            stepn--;
        }
        output[stepm] = carry;
        stepm--;
    }
	for(int i=0;i<9;++i)
		printf("%d",output[i]);
	system("pause");
	return 0;
}