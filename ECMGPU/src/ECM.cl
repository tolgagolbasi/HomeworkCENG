#define NUM 1024
unsigned int ipow(unsigned int base, unsigned int exp)
{
	unsigned int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }

    return result;
}
unsigned int mod_inv(unsigned int a, unsigned int b)
{
	unsigned int t, q;
	unsigned int x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1) {
		q = a / b;
		t = b, b = a % b, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	return x1;
}
long Isqrt (long x){
  long   squaredbit, remainder, root;

   if (x<1) return 0;

   /* Load the binary constant 01 00 00 ... 00, where the number
    * of zero bits to the right of the single one bit
    * is even, and the one bit is as far left as is consistant
    * with that condition.)
    */
   squaredbit  = (long) ((((unsigned long) ~0L) >> 1) &
                        ~(((unsigned long) ~0L) >> 2));
   /* This portable load replaces the loop that used to be
    * here, and was donated by  legalize@xmission.com
    */

   /* Form bits of the answer. */
   remainder = x;  root = 0;
   while (squaredbit > 0) {
     if (remainder >= (squaredbit | root)) {
         remainder -= (squaredbit | root);
         root >>= 1; root |= squaredbit;
     } else {
         root >>= 1;
     }
     squaredbit >>= 2;
   }

   return root;
}
unsigned int gcd(unsigned int a,unsigned int b) {
	unsigned int c;
	  while ( a != 0 ) {
	     c = a;
	     a = b % a;
	     b = c;
	  }
	  return b;
}
unsigned int lcm(unsigned int B) {
	unsigned int a = 2;
	for(unsigned int i=3;i<=B;i++)
		a = a * i / gcd(a,i);
	return a;
}

int gll(unsigned int P[],unsigned int Q[],unsigned int a,unsigned int n) {
	if (P[2] == 0){
		for(unsigned int i=0;i<3;i++)
			P[i]=Q[i];
		return 1;
	}
	if (0 == Q[2]){
		for(int i=0;i<3;i++)
			P[i]=P[i];
		return 1;
	}
	if (P[0] == Q[0]){
		if (P[1] != Q[1]){
			P[1]=1;
			return 1;
		}
		if (P[0] == 0){
			P[1]=1;
			return 1;
		}
		else{
			if (gcd(2*P[0],n)!=1){
				P[2]=2*P[0];
				return 1;
			}
			unsigned int x=(ipow(((3*P[0]*P[0])+a)*(mod_inv(2*P[0],n)),2)-2*P[0]) % n;
			unsigned int y=((((3*P[0]*P[0])+a)*mod_inv(2*P[0],n))*(P[0]-x)-P[1]) % n;
			P[0]=x;
			P[1]=y;
			P[2]=1;
			return 1;
		}
	}
	else{
		if (gcd(Q[0]-P[0],n)!=1){
			P[2]=Q[0]-P[0];
			return 1;
		}
		unsigned int x=(ipow((P[1]-Q[1])*(mod_inv(P[0]-Q[0],n)),2)-P[0]-Q[0]) % n;
		unsigned int y=(((P[1]-Q[1])*mod_inv(P[0]-Q[0],n))*(P[0]-x)-P[1]) % n;
		P[0]=x;
		P[1]=y;
		P[2]=1;
		return 1;
	}
}
void mult2l(unsigned int b,unsigned int P[],unsigned int a,unsigned int n) {
	unsigned int result[3]={0, 1, 0};
	unsigned int pow2P[3];
	for(unsigned int i=0;i<3;i++){
		pow2P[i]=P[i];
	}
	while (b != 0){
		if ((b % 2) == 1){
			gll(pow2P, result, a,n);
		}
		b = b / 2;
		gll(pow2P, pow2P, a,n);
		if ((pow2P[2] > 1) || (pow2P[2] == 0))
			break;
	}
	for(unsigned int i=0;i<3;i++){
		P[i]=pow2P[i];
	}
}
__kernel void Ecm(__global  unsigned int *c,
							 __global  unsigned int *n,
							 const unsigned int Bp)
{
    unsigned int tid = get_global_id(0);
    unsigned int n1 = n[tid];
	unsigned int bounda = 100;
	unsigned int boundb = 100;
	unsigned int y2,y3;
	for(unsigned int a=1;a<=bounda;a++){
		if (((4*a*a*a+27) % n1) == 0)
			continue;
		for(unsigned int b=1; b<=boundb; b++){
			unsigned int x=0;
			do{
				y2=(x*x*x+(a*x)+b) % n1;
				x=x+1;
                y3 = Isqrt(y2);
			} while((y3*y3)!=y2);
			unsigned int fpoint[]={x-1,Isqrt(y2),1};
			mult2l(Bp, fpoint, a, n1);
			if (fpoint[2] == 0)
				break;
			unsigned int g = gcd(fpoint[2], n1);
			if (g > 1){
				c[tid] = g;
       			return;
			}
		}
	}
}
