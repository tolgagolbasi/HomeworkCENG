
#define LEN 8 /* 8*32 = 256 bit */


__kernel void scalarHighMultiply(__global  unsigned int *c,
							 __global  unsigned int *a,
							 __global  unsigned int *b)
{
    unsigned int tid = get_global_id(0);
    
	unsigned int carry;
	unsigned int part;
	unsigned int pro;
	for(int i=0; i<LEN; i++){
		carry = 0;
		pro = a[tid*2*LEN+i+LEN] * b[0];
        part = pro + c[tid*LEN*2+i];
		if (part<pro)
			carry++;
        c[tid*LEN*2+i] = part;		
		carry += mul_hi(a[tid*2*LEN+i+LEN] , b[0]);
        c[tid*LEN*2+i+1] = carry;
    }
}
