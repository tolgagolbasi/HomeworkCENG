#define BYTE_LEN 8
typedef unsigned long uni_t;
#define SIGN uni_t
#define POSITIVE (SIGN)0
#define NEGATIVE (SIGN)1
#define UNIT_LEN (sizeof(uni_t) * BYTE_LEN)
#define UNIT_LEN_HALF (UNIT_LEN >> 1)
#define BOOL uni_t
#define TRUE (BOOL)1
#define FALSE (BOOL)0
typedef uni_t *uni;
#ifndef km_fetch_lo
#define km_fetch_lo(a) ((a) & (((uni_t)1 << UNIT_LEN_HALF) - 1))
#endif
#ifndef km_fetch_hi
#define km_fetch_hi(a) ((uni_t)(a) >> UNIT_LEN_HALF)
#endif
#ifndef km_lo_to_hi
#define km_lo_to_hi(a) ((uni_t)(a) << UNIT_LEN_HALF)
#endif
void km_mul_2(uni_t *zH, uni_t *zL, uni_t *a, uni_t *b){
	uni_t _tLL, _tLH, _tHL, _aL, _bL, _aH, _bH;
	_aL = km_fetch_lo(*a);
	_bL = km_fetch_lo(*b);
	_aH = km_fetch_hi(*a);
	_bH = km_fetch_hi(*b);
	_tLL = _aL * _bL;
	_tHL = _aH * _bL;
	_tLH = _aL * _bH + km_fetch_hi(_tLL) + km_fetch_lo(_tHL);
	(*zL) = km_lo_to_hi(km_fetch_lo(_tLH)) + km_fetch_lo(_tLL);
	(*zH) = _aH * _bH + km_fetch_hi(_tHL) + km_fetch_hi(_tLH);
}
void mam_shift_left_bit(uni_t *cr, uni zn, uni an, uni_t *al, uni_t *nb){
	uni_t _i;
	(*cr) = ((an)[(*al) - 1] >> (UNIT_LEN - (*nb)));
	for (_i = (*al) - 1; _i > 0; _i--){
		(zn)[_i] = ((an)[_i] << (*nb)) | ((an)[_i - 1] >> (UNIT_LEN - (*nb)));
	}
	(zn)[0] = ((an)[0] << (*nb));
}
void km_qr_2(uni_t *q, uni_t *r, uni_t *aH, uni_t *aL, uni_t *b){
	uni_t _qH, _qL, _rH, _rL, _bH, _bL, _t;
	_bL = km_fetch_lo(*b);
	_bH = km_fetch_hi(*b);
	_qH = (*aH) / _bH;
	_rH = (*aH) - (_qH * _bH);
	_rH = km_lo_to_hi(_rH) + km_fetch_hi(*aL);
	_t = _qH * _bL;
	if (_rH < _t){
		_rH += (*b);
		_qH--;
		if (_rH >= (*b)){
			if (_rH < _t){
				_rH += (*b);
				_qH--;
			}
		}
	}
	_rH -= _t;
	_qL = _rH / _bH;
	_rL = _rH - (_qL * _bH);
	_rL = km_lo_to_hi(_rL) + km_fetch_lo(*aL);
	_t = _qL * _bL;
	if (_rL < _t){
		_rL += (*b);
		_qL--;
		if (_rL >= (*b)){
			if (_rL < _t){
				_rL += (*b);
				_qL--;
			}
		}
	}
	(*q) = km_lo_to_hi(_qH) + _qL;
	(*r) = _rL - _t;
}
void mim_q_and_r_1(uni qn, uni rn, uni_t *rl, uni_t *a){
	uni _rhn;
	uni_t _i;
	_rhn = (rn)+1;
	for (_i = (*rl) - 2; _i > 0; _i--){
		km_qr_2(&(qn)[_i], &(rn)[_i], &_rhn[_i], &(rn)[_i], (a));
		_rhn[_i] = 0;
	}
	km_qr_2(&(qn)[0], &(rn)[0], &_rhn[0], &(rn)[0], (a));
	(rn)[1] = 0;
}
void km_mul_2_add_c(uni_t *zH, uni_t *zL, uni_t *a, uni_t *b, uni_t *c){
	uni_t _tLL, _tLH, _tHL, _aL, _bL, _aH, _bH;
	_aL = km_fetch_lo(*a);
	_bL = km_fetch_lo(*b);
	_aH = km_fetch_hi(*a);
	_bH = km_fetch_hi(*b);
	_tLL = _aL * _bL + km_fetch_lo(*c);
	_tHL = _aH * _bL + km_fetch_hi(*c);
	_tLH = _aL * _bH + km_fetch_hi(_tLL) + km_fetch_lo(_tHL);
	(*zL) = km_lo_to_hi(km_fetch_lo(_tLH)) + km_fetch_lo(_tLL);
	(*zH) = _aH * _bH + km_fetch_hi(_tHL) + km_fetch_hi(_tLH);
}
void mam_shift_right_bit(uni zn, uni an, uni_t *al, uni_t *nb){
	uni_t _i;
	for (_i = 0; _i < (*al) - 1; _i++){
		(zn)[_i] = ((an)[_i] >> (*nb)) | ((an)[_i + 1] << (UNIT_LEN - (*nb)));
	}
	(zn)[(*al) - 1] = ((an)[(*al) - 1] >> (*nb));
}
void km_normalize_left(uni_t *digit, uni_t *shift){
	(*shift) = UNIT_LEN - 1;
	while (((*digit) >> (*shift)) != 1){
		(*shift)--;
	}
	(*shift) = UNIT_LEN - 1 - (*shift);
	(*digit) <<= (*shift);
}
void mam_clo(uni zn, uni an, uni_t *al){
	uni_t _i_;
	if ((*al) > 0){
		if ((zn) < (an)){
			for (_i_ = 0; _i_ < (*al); _i_++){
				(zn)[_i_] = (an)[_i_];
			}
		}
		else if ((zn) >(an)){
			for (_i_ = (*al) - 1; _i_ > 0; _i_--){
				(zn)[_i_] = (an)[_i_];
			}
			(zn)[0] = (an)[0];
		}
	}
}
void man_shift_right(uni zn, uni an, uni_t *al, uni_t *n){
	if (n == 0){
		mam_clo(zn, an, al);
	}
	else{
		mam_shift_right_bit(zn, an, al, n);
	}
}
void man_shift_left(uni zn, uni an, uni_t *al, uni_t *n){
	if ((*n) == 0){
		mam_clo(zn, an, al);
		zn[(*al)] = 0;
	}
	else{
		mam_shift_left_bit(&(zn[(*al)]), zn, an, al, n);
	}
}
void min_div_qr(
	uni qn, uni rn,
	const uni an, uni_t al,
	const uni bn, uni_t bl,
	uni nn
	){
		uni_t nph, npl, guessr, guess, tempH, tempL, gH, gL;
		uni_t i, carry, offset, num, shift, rl, nl;
		num = bn[bl - 1];
		km_normalize_left(&num, &shift);

		man_shift_left(rn, an, &al, &shift);
		rl = al;
		if (rn[rl] != 0){
			rl++;
		}
		if ((rn[rl - 1] >> (UNIT_LEN - 1)) == 1){
			rn[rl] = 0;
			rl++;
		}
		if (bl == 1){
			mim_q_and_r_1(qn, rn, &rl, &num);
			if (shift != 0){
				rn[0] >>= shift;
			}
			return;
		}
		else{
			man_shift_left(nn, bn, &bl, &shift);
			nl = bl;
		}
		nph = nn[nl - 1];
		npl = nn[nl - 2];
		while (rl > nl){
			carry = 0;
			if (rn[rl - 1] == nph){
				guess = (uni_t)(-1);
				guessr = nph + rn[rl - 2];
				if (guessr < nph){
					carry = 1;
				}
			}
			else{
				km_qr_2(&guess, &guessr, &rn[rl - 1], &rn[rl - 2], &nph);
			}
			if (carry == 0){
				km_mul_2(&gH, &gL, &guess, &npl);
				if (!((gH < guessr) || ((gH == guessr) && (gL <= rn[rl - 3])))){
					guess--;
					guessr += nph;
					if (guessr >= nph){
						km_mul_2(&gH, &gL, &guess, &npl);
						if (!((gH < guessr) || ((gH == guessr) && (gL <= rn[rl - 3])))){
							guess--;
						}
					}
				}
			}
			carry = 0;
			i = 0;
			offset = (rl - nl - 1);
			while (i < nl){
				km_mul_2_add_c(&tempH, &tempL, &guess, &nn[i], &carry);
				carry = tempH + (rn[offset] < tempL);
				rn[offset] -= tempL;
				offset++;
				i++;
			}
			if (carry > rn[offset]){
				guess--;
				rn[offset] = 0;
				offset = (rl - nl - 1);
				carry = 0;
				i = 0;
				while (i < nl){
					tempL = rn[offset] + nn[i] + carry;
					if (tempL > nn[i]){
						carry = 0;
					}
					else if (tempL < nn[i]){
						carry = 1;
					}
					rn[offset] = tempL;
					offset++;
					i++;
				}
			}
			else{
				rn[offset] -= carry;
			}
			if (qn != 0){
				qn[rl - nl - 1] = guess;
			}
			rl--;
		}
		man_shift_right(rn, rn, &rl, &shift);
}
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
__kernel void mult2l(__global ulong *c, __global  ulong *n, __global  ulong *points, const ushort Bp, const ushort a)
{
    ushort tid = get_global_id(0);
    if(c[tid*2]==1){
        ulong n1[2] = {n[tid*2],n[tid*2+1]};
		ulong pow2P[6]={points[0],0,points[1],0,1,0};
		ulong result[6]={0, 0, 1, 0, 0, 0};
		for(ushort i=2;i<=Bp;i++){
			ushort b=i;
			while (b != 0){
				if ((b % 2) == 1){
					gll(pow2P, result, a,n1);
				}
				b = b / 2;
				gll(pow2P, pow2P, a,n1);
				if ((pow2P[4] > 1) || (pow2P[4] == 0))
					break;
			}
			if ((pow2P[4] > 1) || (pow2P[4] == 0))
				break;
		}
	}
}