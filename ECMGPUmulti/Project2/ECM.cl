void mult(ulong a[],ulong b[],ulong n[],ulong c[]){
	ulong carry;
	ulong part;
	ulong pro;
	for(int i=0; i<2; i++){
        carry = 0;
		for(int j=0; j<2; j++){
			pro = a[i] * b[j];
            part = pro + carry ;
			carry = 0;
			if (part < pro)
				carry++;
			pro = part + c[i+j];
			if (pro < part)
				carry++;
            c[i+j] = pro;
			carry += mul_hi(a[i] , b[j]);
        }
        c[i] = carry;
    }
}
void reduc(ulong a[],ulong b[],ulong n[],ulong c[]){
    ulong temp;
    ulong nu1[2]={0};
    ulong nu2[2*2]={0};
	for(int i=0 ; i<2 ; i++){
		nu1[i] = -1;
	}
	nu1[0] = nu1[0]-b[0];
	for (int i=0 ; i<2*2 ; i++){
		nu2[i]=a[i];
	}
	if((nu2[2]>0)||(nu2[2+1]>0)){
		for(int i=0 ; i<2 ; i++){
			temp = nu2[i] - nu1[i];
			if (temp > nu2[i])
				nu2[i+1]=nu2[i+1]-1;
			nu2[i] = temp;
		}
	}
	for (int i=0 ; i<2*2 ; i++){
		c[i]=nu2[i];
	}
}
void add(ulong a[],ulong b[],ulong n[],ulong c[]){
	ulong temp;
	for(int i=0 ; i<2 ; i++){
		temp = a[i]+b[i];
		if (temp < a[i]){
			c[i+1]++;
		}
		c[i] = c[i]+temp;
	}
}
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

ulong gcd(ulong a[],ulong b[]) {
	return 1;
}
int gll(ulong P[],ulong Q[],uint a,ulong n[]) {
	if (P[4] == 0&&P[5] == 0){
		for(uchar i=0;i<6;i++)
			P[i]=Q[i];
		return 1;
	}
	if (0 == Q[5]&&0 == Q[4]){
		for(uchar i=0;i<6;i++)
			P[i]=P[i];
		return 1;
	}
	if ((P[0] == Q[0])&&(P[1]==Q[1])){
		if ((P[2] != Q[2])||(P[3]!=Q[3])){
			P[0]=1;
			P[1]=0;
			return 1;
		}
		if ((P[0] == 0)&&P[1]==0){
			P[2]=1;
			P[3]=0;
			return 1;
		}
		else{
			ulong x2[2]={P[0],P[1]};
			ulong x1[2]={0, 0};
			mult(2,x2,n,x1);
			if (gcd(x1,n)!=1){
				P[5]=x1[1];
				P[4]=x1[0];
				return 1;
			}
			mult(x2, x2, n, x1);
			mult(3, x1, n, x1);
			add(x1, a, n, x1);
			ulong x[2]={0,0};
			ulong y[2]={0,0};
			ulong x3[2]={0, 0};
			mult(2,x2,n,x3);
	//		ulong x=(ipow(x1*(mod_inv(x3,n)),2)-x3);
			mult(3, x2, n, x1);
			add(x1, a, n, x1);
	//		ulong y=((x1*mod_inv(x3,n))*(P[0]-x)-P[1]);
			P[0]=x[0];
			P[1]=x[1];
			P[2]=y[0];
			P[3]=y[1];
			P[4]=1;
			P[5]=0;
			return 1;
		}
	}
	else{
		ulong x1[2]={Q[0],Q[1]};
		ulong x2[2]={P[0],P[1]};
		ulong x3[2]={Q[2],Q[3]};
		ulong x4[2]={P[2],P[3]};
		ulong x5[2]={0,0};
		reduc(x1,x2,n,x5);
		if (gcd(x5,n)!=1){
			P[4]=x5[0];
			P[5]=x5[1];
			return 1;
		}
		ulong x6[2]={0,0};
		ulong x[2]={0,0};
		ulong y[2]={0,0};
		reduc(x4,x3,n,x5);
		reduc(x2,x1,n,x6);
	//	ulong x=(ipow(x5*(mod_inv(x6,n)),2)-P[0]-Q[0]);
	//	ulong y=((x5*mod_inv(x6,n))*(P[0]-x)-P[1]);
		P[0]=x[0];
		P[1]=x[1];
		P[2]=y[0];
		P[3]=y[1];
		P[4]=1;
		P[5]=0;
		return 1;
	}
}
void mult2lo(unsigned int b1,unsigned long P[],unsigned int a,unsigned long n[]) {
	ulong result[6]={0, 0, 1, 0, 0, 0};
	ulong pow2P[6];
	for(uchar i=0;i<6;i++){
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
	for(uchar i=0;i<6;i++){
		P[i]=pow2P[i];
	}
}
__kernel void mult2l(__global ulong *c, __global  ulong *n, __global  ulong *points, const ushort Bp, const ushort a)
{
    ushort tid = get_global_id(0);
    ulong n1[2] = {n[tid*2],n[tid*2+1]};
	ulong fpoint[6]={x-1,0,y3,0,1,0};
	mult2lo(Bp, fpoint, a, n1);
}