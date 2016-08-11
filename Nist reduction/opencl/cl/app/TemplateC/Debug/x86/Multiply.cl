/**********************************************************************
Copyright ©2012 Advanced Micro Devices, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

•	Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
•	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or
 other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************/

/*!
 * Sample kernel which multiplies every element of the input array with
 * a constant and stores it at the corresponding output array
 */
#define LEN 8 /* 8*32 = 256 bit */

__kernel void multiply(__global  unsigned int *c,
							 __global  unsigned int *a,
							 __global  unsigned int *b)
{
    unsigned int tid = get_global_id(0);
        
    unsigned int nu1[LEN];
    unsigned int nu2[LEN];
    unsigned int co[LEN*2]={0};

	for(int i=0 ; i<LEN ; i++){
		nu1[i] = a[tid*LEN+i];
		nu2[i] = b[tid*LEN+i];
	}
    unsigned short stepm = LEN; 
	unsigned short stepn;
	unsigned int carry;
	unsigned int part;
	unsigned int pro;
    while( stepm > 0 ){
        stepn = LEN;
        carry = 0;
        while( stepn > 0 ){
			pro =nu1[stepm-1] * nu2[stepn-1];
            part = pro + carry ;
			carry = 0;
			if (part < pro)
				carry++;
			pro = part + co[stepm + stepn-1];
			if (pro < part)
				carry++;
            co[stepm + stepn-1] = pro;
			carry += mul_hi(nu1[stepm-1] , nu2[stepn-1]);
            stepn--;
        }
        co[stepm-1] = carry;
        stepm--;
    }
	for (int i=0 ; i<2*LEN ; i++){
		c[tid*2*LEN+i] = co[i];
	}
}
