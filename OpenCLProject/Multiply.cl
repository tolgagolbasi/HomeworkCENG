#define LEN 8 /* 8*32 = 256 bit */

__kernel void multiply(__global  unsigned int *c,
							 __global  unsigned int *a,
							 __global  unsigned int *b)
{
    unsigned int tid = get_global_id(0);
	unsigned int carry;
	unsigned int part;
	unsigned int pro;
	for(int i=0; i<LEN; i++){
        carry = 0;
		for(int j=0; j<LEN; j++){
			pro = a[tid*LEN+i] * b[tid*LEN+j];
            part = pro + carry ;
			carry = 0;
			if (part < pro)
				carry++;
			pro = part + c[tid*2*LEN+i+j];
			if (pro < part)
				carry++;
            c[tid*2*LEN+i+j] = pro;
			carry += mul_hi(a[tid*LEN+i] , b[tid*LEN+j]);
        }
        c[tid*2*LEN+i] = carry;
    }
}
