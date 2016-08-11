#define LEN 8 /* 8*32 = 256 bit */

__kernel void addLow(__global  unsigned int *c,
							 __global  unsigned int *a,
							 __global  unsigned int *b)
{
    unsigned int tid = get_global_id(0);
	unsigned int temp;
	for(int i=0 ; i<LEN ; i++){
		temp = a[tid*LEN*2+i]+b[tid*LEN*2+i];
		if (temp < a[tid*LEN*2+i]){
			c[tid*LEN*2+i+1]++;
		}
		c[tid*LEN*2+i] = c[tid*LEN*2+i]+temp;
	}
	c[tid*LEN*2+LEN]+=b[tid*LEN*2+LEN];
	c[tid*LEN*2+LEN]+=b[tid*LEN*2+LEN+1];
}
