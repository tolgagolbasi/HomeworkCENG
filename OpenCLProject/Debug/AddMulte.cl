#define LEN 8 /* 8*32 = 256 bit */

__kernel void addMulte(__global  unsigned int *c,
				  __global  unsigned int *a,
				  __global  unsigned int *b)
{
    unsigned int tid = get_global_id(0);
	unsigned int temp;
	unsigned int carry;
	unsigned int part;
	unsigned int pro;
	for(int i=0; i<LEN ; i++){
        carry = 0;
		pro = a[tid*LEN*2+LEN+i] * b[0];
        part = pro + c[tid*LEN*2+i] ;
		if (part < pro)
			carry++;
        c[tid*LEN*2+i] = part;
		carry += mul_hi(a[tid*LEN*2+LEN+i] , b[0]);
        c[tid*LEN*2+i+1] = carry;
    }
	for(int i=0 ; i<LEN ; i++){
		temp = c[tid*2*LEN+i]+a[tid*LEN*2+i];
		if (temp < c[tid*2*LEN+i]){
			c[tid*2*LEN+i+1]++;
		}
		c[tid*2*LEN+i] += temp;
	}
}