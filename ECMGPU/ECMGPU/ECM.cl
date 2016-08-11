ulong ipow(ulong base, ulong exp)
{
	ulong result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}
ulong mod_inv(ulong a, ulong b)
{
	ulong t, q;
	ulong x0 = 0, x1 = 1;
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
   squaredbit  = (long) ((((unsigned long) ~0L) >> 1) &
                        ~(((unsigned long) ~0L) >> 2));
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
ulong gcd(ulong a,ulong b) {
	ulong c;
	  while ( a != 0 ) {
	     c = a;
	     a = b % a;
	     b = c;
	  }
	  return b;
}
int gll(ulong P[],ulong Q[],uint a,ulong n) {
	if (P[2] == 0){
		for(uchar i=0;i<3;i++)
			P[i]=Q[i];
		return 1;
	}
	if (0 == Q[2]){
		for(uchar i=0;i<3;i++)
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
			ulong x=(ipow(((3*P[0]*P[0])+a)*(mod_inv(2*P[0],n)),2)-2*P[0]) % n;
			ulong y=((((3*P[0]*P[0])+a)*mod_inv(2*P[0],n))*(P[0]-x)-P[1]) % n;
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
		ulong x=(ipow((P[1]-Q[1])*(mod_inv(P[0]-Q[0],n)),2)-P[0]-Q[0]) % n;
		ulong y=(((P[1]-Q[1])*mod_inv(P[0]-Q[0],n))*(P[0]-x)-P[1]) % n;
		P[0]=x;
		P[1]=y;
		P[2]=1;
		return 1;
	}
}
void mult2l(unsigned int b1,unsigned long P[],unsigned int a,unsigned long n) {
	ulong result[3]={0, 1, 0};
	ulong pow2P[3];
	for(uchar i=0;i<3;i++){
		pow2P[i]=P[i];
	}
	for(ushort i=2;i<=b1;i++){
		ushort b=i;
		while (b != 0){
			if ((b % 2) == 1){
				gll(pow2P, result, a,n);
			}
			b = b / 2;
			gll(pow2P, pow2P, a,n);
			if ((pow2P[2] > 1) || (pow2P[2] == 0))
				break;
		}
		if ((pow2P[2] > 1) || (pow2P[2] == 0))
			break;
	}
	for(uchar i=0;i<3;i++){
		P[i]=pow2P[i];
	}
}
__kernel void Ecm(__global  ulong *c,
							 __global  ulong *n,
							 const ushort Bp)
{
    ushort tid = get_global_id(0);
    ulong n1 = n[tid];
	uchar bounda = 5;
	uchar boundb = 5;
	ulong y2,y3;
	uchar a=0,b,x;
	do{
		a+=1;
		if (((4*a*a*a+27) % n1) == 0)
		continue;
		for(b=1; b<=boundb; b++){
			x=0;
			do{
				y2=(x*x*x+(a*x)+b) % n1;
				x=x+1;
                y3 = Isqrt(y2);
			} while(((y3*y3)!=y2)&&(x<25));
			if(x>=25){
				continue;
			}
			ulong fpoint[]={x-1,y3,1};
			mult2l(Bp, fpoint, a, n1);
			if (fpoint[2] == 0){
				break;
			}
			ulong g = gcd(fpoint[2], n1);
			if (g > 1){
				c[tid] = g;
       			return;
			}
		}
	}while(a < bounda);
}