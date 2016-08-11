#define LEN 8 /* 8*32 = 256 bit */

__kernel void reduce(__global  unsigned int *c,
							 __global  unsigned int *a,
								__global  unsigned int *b)
{
    unsigned int tid = get_global_id(0);
    unsigned int temp;
	for(int i=0 ; i<LEN ; i++){
		temp = a[tid*LEN+i] - b[i];
		c[tid*LEN+i] = temp;
		if (temp > b[i])
			c[tid*LEN+i+1]=c[tid*LEN+i+1]-1;
	}
}
