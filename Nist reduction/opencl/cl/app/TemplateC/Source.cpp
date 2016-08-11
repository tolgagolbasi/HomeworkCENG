#include <string.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>
int main()
{
	clock_t begin, end;
	double time_spent;
	unsigned long input1[2560000];
	unsigned long input2[2560000];
	unsigned long output[2560000];
    bool passed = true;
	begin = clock();
    for(unsigned int i = 0; i < 2560000; ++i)
        output[i] = input1[i] * input2[i];
	end = clock();	
}