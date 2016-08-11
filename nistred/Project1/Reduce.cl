
#define LEN 8 /* 8*32 = 256 bit */

__kernel void reduce(__global  unsigned int *c,
					 __global  unsigned int *a,
					 __global  unsigned int *b)
{
    unsigned int tid = get_global_id(0);
    unsigned int temp;
    unsigned int nu1[LEN]={0};
    unsigned int nu2[2*LEN]={0};
	for(int i=0 ; i<LEN ; i++){
		nu1[i] = -1;
	}
	nu1[0] = nu1[0]-b[0];
	for (int i=0 ; i<2*LEN ; i++){
		nu2[i]=a[tid*2*LEN+i];
	}
	if((nu2[LEN]>0)||(nu2[LEN+1]>0)){
		for(int i=0 ; i<LEN ; i++){
			temp = nu2[i] - nu1[i];
			if (temp > nu2[i])
				nu2[i+1]=nu2[i+1]-1;
			nu2[i] = temp;
		}
	}
	for (int i=0 ; i<2*LEN ; i++){
		c[tid*2*LEN+i]=nu2[i];
	}
}
