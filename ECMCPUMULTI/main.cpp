#include <stdio.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include <stdint.h>
#include <gmp.h>
#include <stdlib.h>
using namespace std;
#define BYTE_LEN 8
typedef unsigned long uni_t;
#define POSITIVE (SIGN)0
#define NEGATIVE (SIGN)1

#define UNIT_LEN (sizeof(uni_t) * BYTE_LEN)
#define UNIT_LEN_HALF (UNIT_LEN >> 1)
#define THRESHOLD_GCD_LEHMER 16
#define LO 0
#define HI 1

#define BOOL uni_t
#define TRUE (BOOL)1
#define FALSE (BOOL)0

#define SIGN uni_t
typedef uni_t *uni;
#ifndef mim_sign_inv
#define mim_sign_inv(a) (((a) == 0))
#endif
#define CHECK(__a__)if(1){ \
	if ((__a__) == FALSE){\
	} \
}

void km_log_sub(uni_t *z, uni_t *a, uni_t *b){
	uni_t _t;
	for (_t = 0; ((*a) >> _t) >= (*b); _t++);
	(*z) = _t - 1;
}
void km_add_c(uni_t *cr, uni_t *z, uni_t *a, uni_t *b, uni_t *c){
	uni_t _t;
	_t = (*a) + (*b) + (*c);
	if (_t < (*a)){
		(*cr) = 1;
	}
	else if (_t >(*a)){
		(*cr) = 0;
	}
	(*z) = _t;
}
void mam_clr(uni an, uni_t al){
	uni_t _i;
	for (_i = 0; _i < (al); _i++){
		(an)[_i] = 0;
	}
}
void km_sub(uni_t *cr, uni_t *z, uni_t *a, uni_t *b){
	(*cr) = ((*a) < (*b));
	(*z) = (*a) - (*b);
}
#ifndef km_fetch_lo
#define km_fetch_lo(a) ((a) & (((uni_t)1 << UNIT_LEN_HALF) - 1))
#endif
#ifndef km_fetch_hi
#define km_fetch_hi(a) ((uni_t)(a) >> UNIT_LEN_HALF)
#endif
#ifndef km_lo_to_hi
#define km_lo_to_hi(a) ((uni_t)(a) << UNIT_LEN_HALF)
#endif
void km_sub_c(uni_t *cr, uni_t *z, uni_t *a, uni_t *b, uni_t *c){
	uni_t _t;
	_t = (*a) - (*b) - (*c);
	if (_t < (*a)){
		(*cr) = 0;
	}
	else if (_t >(*a)){
		(*cr) = 1;
	}
	(*z) = _t;
}
void km_add(uni_t *cr, uni_t *z, uni_t *a, uni_t *b){
	(*z) = (*a) + (*b);
	(*cr) = ((*z) < (*a));
}
void km_sub_2(uni_t *zH, uni_t *zL, uni_t *aH, uni_t *aL, uni_t *b){
	uni_t _tH, _tL;
	km_sub(&_tH, &_tL, aL, b);
	(*zH) = (*aH) - _tH;
	(*zL) = _tL;
}
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
void km_add_2(uni_t *zH, uni_t *zL, uni_t *aH, uni_t *aL, uni_t *b){
	uni_t _tH, _tL;
	km_add(&_tH, &_tL, aL, b);
	(*zH) = (*aH) + _tH;
	(*zL) = _tL;
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
void mim_mul_1(uni_t *cr, uni zn, uni an, uni_t *al, uni_t *b, uni_t *c){
	uni_t _i, _t;
	_t = (*c);
	_i = (*al) & 0x7;
	if (_i < 4){
		if (_i == 1){
			km_mul_2_add_c(&_t, &(zn)[0], &(an)[0], (b), &_t);
		}
		else if (_i == 2){
			km_mul_2_add_c(&_t, &(zn)[0], &(an)[0], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[1], &(an)[1], (b), &_t);
		}
		else if (_i == 3){
			km_mul_2_add_c(&_t, &(zn)[0], &(an)[0], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[1], &(an)[1], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[2], &(an)[2], (b), &_t);
		}
	}
	else if (_i > 4){
		if (_i == 5){
			km_mul_2_add_c(&_t, &(zn)[0], &(an)[0], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[1], &(an)[1], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[2], &(an)[2], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[3], &(an)[3], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[4], &(an)[4], (b), &_t);
		}
		else if (_i == 6){
			km_mul_2_add_c(&_t, &(zn)[0], &(an)[0], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[1], &(an)[1], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[2], &(an)[2], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[3], &(an)[3], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[4], &(an)[4], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[5], &(an)[5], (b), &_t);
		}
		else{
			km_mul_2_add_c(&_t, &(zn)[0], &(an)[0], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[1], &(an)[1], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[2], &(an)[2], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[3], &(an)[3], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[4], &(an)[4], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[5], &(an)[5], (b), &_t);
			km_mul_2_add_c(&_t, &(zn)[6], &(an)[6], (b), &_t);
		}
	}
	else{
		km_mul_2_add_c(&_t, &(zn)[0], &(an)[0], (b), &_t);
		km_mul_2_add_c(&_t, &(zn)[1], &(an)[1], (b), &_t);
		km_mul_2_add_c(&_t, &(zn)[2], &(an)[2], (b), &_t);
		km_mul_2_add_c(&_t, &(zn)[3], &(an)[3], (b), &_t);
	}
	for (; _i < (*al); _i++){
		km_mul_2_add_c(&_t, &(zn)[_i], &(an)[_i], (b), &_t);
		_i++;
		km_mul_2_add_c(&_t, &(zn)[_i], &(an)[_i], (b), &_t);
		_i++;
		km_mul_2_add_c(&_t, &(zn)[_i], &(an)[_i], (b), &_t);
		_i++;
		km_mul_2_add_c(&_t, &(zn)[_i], &(an)[_i], (b), &_t);
		_i++;
		km_mul_2_add_c(&_t, &(zn)[_i], &(an)[_i], (b), &_t);
		_i++;
		km_mul_2_add_c(&_t, &(zn)[_i], &(an)[_i], (b), &_t);
		_i++;
		km_mul_2_add_c(&_t, &(zn)[_i], &(an)[_i], (b), &_t);
		_i++;
		km_mul_2_add_c(&_t, &(zn)[_i], &(an)[_i], (b), &_t);
	}
	(*cr) = _t;
}

void km_mul_2_add_2(uni_t *zH, uni_t *zL, uni_t *a, uni_t *b, uni_t *c, uni_t *d){
	uni_t _tLL, _tLH, _tHL, _aL, _bL, _aH, _bH;
	_aL = km_fetch_lo(*a);
	_bL = km_fetch_lo(*b);
	_aH = km_fetch_hi(*a);
	_bH = km_fetch_hi(*b);
	_tLL = _aL * _bL + km_fetch_lo(*c) + km_fetch_lo(*d);
	_tHL = _aH * _bL + km_fetch_hi(*c) + km_fetch_hi(*d);
	_tLH = _aL * _bH + km_fetch_hi(_tLL) + km_fetch_lo(_tHL);
	(*zL) = km_lo_to_hi(km_fetch_lo(_tLH)) + km_fetch_lo(_tLL);
	(*zH) = _aH * _bH + km_fetch_hi(_tHL) + km_fetch_hi(_tLH);
}
void mim_inc_n_mul_1(uni_t *cr, uni zn, uni an, uni_t *al, uni_t *b, uni_t *c){
	uni_t _i, _t;
	_t = (*c);
	_i = (*al) & 0x7;
	if (_i < 4){
		if (_i == 1){
			km_mul_2_add_2(&_t, &(zn)[0], &(an)[0], (b), &_t, &(zn)[0]);
		}
		else if (_i == 2){
			km_mul_2_add_2(&_t, &(zn)[0], &(an)[0], (b), &_t, &(zn)[0]);
			km_mul_2_add_2(&_t, &(zn)[1], &(an)[1], (b), &_t, &(zn)[1]);
		}
		else if (_i == 3){
			km_mul_2_add_2(&_t, &(zn)[0], &(an)[0], (b), &_t, &(zn)[0]);
			km_mul_2_add_2(&_t, &(zn)[1], &(an)[1], (b), &_t, &(zn)[1]);
			km_mul_2_add_2(&_t, &(zn)[2], &(an)[2], (b), &_t, &(zn)[2]);
		}
	}
	else if (_i > 4){
		if (_i == 5){
			km_mul_2_add_2(&_t, &(zn)[0], &(an)[0], (b), &_t, &(zn)[0]);
			km_mul_2_add_2(&_t, &(zn)[1], &(an)[1], (b), &_t, &(zn)[1]);
			km_mul_2_add_2(&_t, &(zn)[2], &(an)[2], (b), &_t, &(zn)[2]);
			km_mul_2_add_2(&_t, &(zn)[3], &(an)[3], (b), &_t, &(zn)[3]);
			km_mul_2_add_2(&_t, &(zn)[4], &(an)[4], (b), &_t, &(zn)[4]);
		}
		else if (_i == 6){
			km_mul_2_add_2(&_t, &(zn)[0], &(an)[0], (b), &_t, &(zn)[0]);
			km_mul_2_add_2(&_t, &(zn)[1], &(an)[1], (b), &_t, &(zn)[1]);
			km_mul_2_add_2(&_t, &(zn)[2], &(an)[2], (b), &_t, &(zn)[2]);
			km_mul_2_add_2(&_t, &(zn)[3], &(an)[3], (b), &_t, &(zn)[3]);
			km_mul_2_add_2(&_t, &(zn)[4], &(an)[4], (b), &_t, &(zn)[4]);
			km_mul_2_add_2(&_t, &(zn)[5], &(an)[5], (b), &_t, &(zn)[5]);
		}
		else{
			km_mul_2_add_2(&_t, &(zn)[0], &(an)[0], (b), &_t, &(zn)[0]);
			km_mul_2_add_2(&_t, &(zn)[1], &(an)[1], (b), &_t, &(zn)[1]);
			km_mul_2_add_2(&_t, &(zn)[2], &(an)[2], (b), &_t, &(zn)[2]);
			km_mul_2_add_2(&_t, &(zn)[3], &(an)[3], (b), &_t, &(zn)[3]);
			km_mul_2_add_2(&_t, &(zn)[4], &(an)[4], (b), &_t, &(zn)[4]);
			km_mul_2_add_2(&_t, &(zn)[5], &(an)[5], (b), &_t, &(zn)[5]);
			km_mul_2_add_2(&_t, &(zn)[6], &(an)[6], (b), &_t, &(zn)[6]);
		}
	}
	else{
		km_mul_2_add_2(&_t, &(zn)[0], &(an)[0], (b), &_t, &(zn)[0]);
		km_mul_2_add_2(&_t, &(zn)[1], &(an)[1], (b), &_t, &(zn)[1]);
		km_mul_2_add_2(&_t, &(zn)[2], &(an)[2], (b), &_t, &(zn)[2]);
		km_mul_2_add_2(&_t, &(zn)[3], &(an)[3], (b), &_t, &(zn)[3]);
	}
	for (; _i < (*al); _i++){
		km_mul_2_add_2(&_t, &(zn)[_i], &(an)[_i], (b), &_t, &(zn)[_i]);
		_i++;
		km_mul_2_add_2(&_t, &(zn)[_i], &(an)[_i], (b), &_t, &(zn)[_i]);
		_i++;
		km_mul_2_add_2(&_t, &(zn)[_i], &(an)[_i], (b), &_t, &(zn)[_i]);
		_i++;
		km_mul_2_add_2(&_t, &(zn)[_i], &(an)[_i], (b), &_t, &(zn)[_i]);
		_i++;
		km_mul_2_add_2(&_t, &(zn)[_i], &(an)[_i], (b), &_t, &(zn)[_i]);
		_i++;
		km_mul_2_add_2(&_t, &(zn)[_i], &(an)[_i], (b), &_t, &(zn)[_i]);
		_i++;
		km_mul_2_add_2(&_t, &(zn)[_i], &(an)[_i], (b), &_t, &(zn)[_i]);
		_i++;
		km_mul_2_add_2(&_t, &(zn)[_i], &(an)[_i], (b), &_t, &(zn)[_i]);
	}
	(*cr) = _t;
}
void mam_shift_right_bit(uni zn, uni an, uni_t *al, uni_t *nb){
	uni_t _i;
	for (_i = 0; _i < (*al) - 1; _i++){
		(zn)[_i] = ((an)[_i] >> (*nb)) | ((an)[_i + 1] << (UNIT_LEN - (*nb)));
	}
	(zn)[(*al) - 1] = ((an)[(*al) - 1] >> (*nb));
}
void mim_inc_n(uni_t *cr, uni zn, uni_t *zl, uni an, uni_t *al, uni_t *pad){
	uni_t _i;
	(*cr) = (*pad);
	if ((*zl) >(*al)){
		for (_i = 0; _i < (*al); _i++){
			km_add_c((cr), &(zn)[_i], &(zn)[_i], &(an)[_i], (cr));
		}
		for (_i = (*al); _i < (*zl); _i++){
			km_add((cr), &(zn)[_i], &(zn)[_i], (cr));
		}
	}
	else{
		for (_i = 0; _i < (*zl); _i++){
			km_add_c((cr), &(zn)[_i], &(zn)[_i], &(an)[_i], (cr));
		}
		for (_i = (*zl); _i < (*al); _i++){
			km_add((cr), &(zn)[_i], &(an)[_i], (cr));
		}
	}
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
void mam_trim(uni an, uni_t *al){
	while (((an)[(*al) - 1] == 0) && ((*al) > 1)){
		(*al)--;
	}
}
void min_mul_basecase(
	uni zn,
	const uni an, uni_t *al,
	const uni bn, uni_t *bl
	){
		uni_t i;

		mim_mul_1(&zn[(*al)], zn, an, al, &bn[0], 0);
		for (i = 1; i < (*bl); i++){
			mim_inc_n_mul_1(&zn[i + (*al)], (zn + i), an, al, &bn[i], 0);
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

static uni_t div_lehmer(uni_t *aH, uni_t *aL, uni_t *bH, uni_t *bL){
	uni_t q, sh, tH, tL;
	q = (*aH) / ((*bH) + 1);
	if (q != (((*aH) + 1) / (*bH))){
		q = 0;
		while (((*aH) > (*bH)) || (((*aH) == (*bH)) && ((*aL) >= (*bL)))){
			km_log_sub(&sh, aH, bH);
			if (sh != 0){
				tH = ((*bH) << sh) | ((*bL) >> (UNIT_LEN - sh));
				tL = ((*bL) << sh);
			}
			else{
				tH = (*bH);
				tL = (*bL);
			}
			if ((tH > (*aH)) || ((tH == (*aH)) && (tL >= (*aL)))){
				sh--;
				tL = (tH << (UNIT_LEN - 1)) + (tL >> 1);
				tH >>= 1;
			}
			q |= ((uni_t)1 << sh);
			km_sub_2(aH, aL, aH, aL, &tL);
			(*aH) -= tH;
		}
	}
	return q;
}
void min_gcdx_lehmer(
	uni dn,
	uni xn, uni_t xl, uni yn, uni_t yl
	){
		uni_t tDH, tDL, tq0, tq1;
		uni_t tq[4];
		uni_t txxH, txxL, tyxH, tyxL, txyH, txyL, tyyH, tyyL, crx, cry;
		uni_t i, tl, tql, dl, shift, A, B, C, D, q, t;
		uni_t uaH, uaL, vaH, vaL, tAH, tAL, tCH, tCL, tBH, tBL;
		SIGN sign = POSITIVE;
		dl = xl, yn[yl] = 0;
		while (!((yl == 1) && (yn[0] == 0))){
			A = 1; B = 0; C = 0; D = 1;
			uaH = xn[xl - 1];
			km_normalize_left(&uaH, &shift);
			if (xl > THRESHOLD_GCD_LEHMER){
				if (shift != 0){
					uaH += xn[xl - 2] >> (UNIT_LEN - shift);
					uaL = (xn[xl - 2] << shift) + (xn[xl - 3] >> (UNIT_LEN - shift));
					vaH = (yn[xl - 1] << shift) + (yn[xl - 2] >> (UNIT_LEN - shift));
					vaL = (yn[xl - 2] << shift) + (yn[xl - 3] >> (UNIT_LEN - shift));
				}
				else{
					uaL = xn[xl - 2];
					vaH = yn[xl - 1];
					vaL = yn[xl - 2];
				}
				while (TRUE){
					sign = POSITIVE;
					km_sub_2(&tCH, &tCL, &vaH, &vaL, &C);
					if (tCH == 0){ break; }
					km_add_2(&tDH, &tDL, &vaH, &vaL, &D);
					if (tDH == 0){ break; }
					km_add_2(&tAH, &tAL, &uaH, &uaL, &A);
					tq0 = div_lehmer(&tAH, &tAL, &tCH, &tCL);
					km_sub_2(&tBH, &tBL, &uaH, &uaL, &B);
					tq1 = div_lehmer(&tBH, &tBL, &tDH, &tDL);
					if (tq0 != tq1){ break; }
					tq1 = (tq0 * C) + A, A = C, C = tq1;
					tq1 = (tq0 * D) + B, B = D, D = tq1;
					km_mul_2(&tAH, &tAL, &vaL, &tq0);
					tAH += (vaH * tq0);
					km_sub_2(&tq1, &tq0, &uaH, &uaL, &tAL);
					tq1 -= tAH;
					uaH = vaH, uaL = vaL, vaH = tq1, vaL = tq0;
					sign = NEGATIVE;
					km_add_2(&tCH, &tCL, &vaH, &vaL, &C);
					km_sub_2(&tDH, &tDL, &vaH, &vaL, &D);
					if (tDH == 0){ break; }
					km_sub_2(&tAH, &tAL, &uaH, &uaL, &A);
					tq0 = div_lehmer(&tAH, &tAL, &tCH, &tCL);
					km_add_2(&tBH, &tBL, &uaH, &uaL, &B);
					tq1 = div_lehmer(&tBH, &tBL, &tDH, &tDL);
					if (tq0 != tq1){ break; }
					tq1 = (tq0 * C) + A, A = C, C = tq1;
					tq1 = (tq0 * D) + B, B = D, D = tq1;
					km_mul_2(&tAH, &tAL, &vaL, &tq0);
					tAH += (vaH * tq0);
					km_sub_2(&tq1, &tq0, &uaH, &uaL, &tAL);
					tq1 -= tAH;
					uaH = vaH, uaL = vaL, vaH = tq1, vaL = tq0;
				}
			}
			else{
				if (shift != 0){
					if (xl > 1){
						uaH += xn[xl - 2] >> (UNIT_LEN - shift);
						vaH = (yn[xl - 1] << shift) + (yn[xl - 2] >> (UNIT_LEN - shift));
					}
					else{
						vaH = (yn[xl - 1] << shift);
					}
				}
				else{
					vaH = yn[xl - 1];
				}
				while (TRUE){
					sign = POSITIVE;
					if ((vaH - C) == 0){ break; }
					if ((vaH + D) == 0){ break; }
					q = ((uaH + A) / (vaH - C));
					if (q != ((uaH - B) / (vaH + D))){ break; }
					t = A + (q * C), A = C, C = t;
					t = B + (q * D), B = D, D = t;
					t = uaH - (q * vaH), uaH = vaH, vaH = t;
					sign = NEGATIVE;
					if ((vaH - D) == 0){ break; }
					q = ((uaH - A) / (vaH + C));
					if (q != ((uaH + B) / (vaH - D))){ break; }
					t = A + (q * C), A = C, C = t;
					t = B + (q * D), B = D, D = t;
					t = uaH - (q * vaH), uaH = vaH, vaH = t;
				}
			}
			if (B == 0){
				//uni tq, tt;
				//tq = (uni)kn_init_fast();
				//tt = (uni)kn_init_fast();
				mam_clo(dn, xn, &xl);
				mam_clo(xn, yn, &yl);
				mam_clo(yn, dn, &xl);
				mam_clr(xn + yl, xl - yl);
				tl = xl, xl = yl, yl = tl;
				tq[yl - xl] = 0;
				min_div_qr(tq, yn, yn, yl, xn, xl, dn);
				tql = yl - xl + 1;
				mam_trim(tq, &tql);
				//kn_free_fast((void *)tq);
				//kn_free_fast((void *)tt);
			}
			else{
				crx = cry = txxH = tyxH = txyH = tyyH = 0;
				if (sign == POSITIVE){
					for (i = 0; i < xl; i++){
						km_mul_2_add_c(&txxH, &txxL, &A, &xn[i], &txxH);
						km_mul_2_add_c(&tyxH, &tyxL, &B, &yn[i], &tyxH);
						km_mul_2_add_c(&txyH, &txyL, &C, &xn[i], &txyH);
						km_mul_2_add_c(&tyyH, &tyyL, &D, &yn[i], &tyyH);
						km_sub_c(&cry, &yn[i], &tyyL, &txyL, &cry);
						km_sub_c(&crx, &xn[i], &txxL, &tyxL, &crx);
					}
					xn[i] = txxH - tyxH - crx;
					yn[i] = tyyH - txyH - cry;
				}
				else{
					for (i = 0; i < xl; i++){
						km_mul_2_add_c(&txxH, &txxL, &A, &xn[i], &txxH);
						km_mul_2_add_c(&tyxH, &tyxL, &B, &yn[i], &tyxH);
						km_mul_2_add_c(&txyH, &txyL, &C, &xn[i], &txyH);
						km_mul_2_add_c(&tyyH, &tyyL, &D, &yn[i], &tyyH);
						km_sub_c(&cry, &yn[i], &txyL, &tyyL, &cry);
						km_sub_c(&crx, &xn[i], &tyxL, &txxL, &crx);
					}
					xn[i] = tyxH - txxH - crx;
					yn[i] = txyH - tyyH - cry;
				}
			}
			mam_trim(xn, &xl);
			mam_trim(yn, &yl);
		}
		mam_clo(dn, xn, &dl);
}
unsigned long ipow(unsigned long base, unsigned long exp)
{
	unsigned long result = 1;
	while (exp)
	{
		if (exp & 1)
			result *= base;
		exp >>= 1;
		base *= base;
	}

	return result;
}
void add(uni c,uni a,uni b){
	uni_t temp;
	for(int i=0 ; i<2 ; i++){
		temp = a[i]+b[i];
		if (temp < a[i]){
			c[i+1]++;
		}
		c[i] = c[i]+temp;
	}
}
void sub(uni c,uni a,uni b){
    uni_t temp;
    uni_t nu1[2]={0};
    uni_t nu2[2*2]={0};
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
void mult(uni c,uni a,uni b,uni n){
	uni_t carry;
	uni_t part;
	uni_t pro;
	uni_t zh,zl;
	for(int i=0; i<2; i++){
        carry = 0;
		for(int j=0; j<2; j++){
			km_mul_2(&zh,&zl,&a[i],&b[j]);
			pro = zl;
            part = pro + carry ;
			carry = 0;
			if (part < pro)
				carry++;
			pro = part + c[i+j];
			if (pro < part)
				carry++;
            c[i+j] = pro;
			carry +=zh;
        }
        c[i] = carry;
    }
	uni_t qn[2],rn[2];
	uni nn;
	min_div_qr(qn,rn,c,4,n,2,nn);
	c[0]=rn[0];
	c[1]=rn[1];
}
void mod_inv(uni c,uni a, uni b)
{
	mpz_t x;
	mpz_t y;
	mpz_t z;
	mpz_init(x);
	mpz_init(y);
	mpz_init(z);
	mpz_set_ui(z,0);
	mpz_import(x, 2, -1, sizeof(a[0]), 0, 0, a);
	mpz_import(y, 2, -1, sizeof(b[0]), 0, 0, b);
	mpz_invert(z, x, y);
	mpz_export(c, NULL, -1, sizeof(a[0]), 0, 0, z);
}
long Isqrt(long x){
	long   squaredbit, remainder, root;

	if (x < 1) return 0;

	/* Load the binary constant 01 00 00 ... 00, where the number
	* of zero bits to the right of the single one bit
	* is even, and the one bit is as far left as is consistant
	* with that condition.)
	*/
	squaredbit = (long)((((unsigned long)~0L) >> 1) &
		~(((unsigned long)~0L) >> 2));
	/* This portable load replaces the loop that used to be
	* here, and was donated by  legalize@xmission.com
	*/

	/* Form bits of the answer. */
	remainder = x;  root = 0;
	while (squaredbit > 0) {
		if (remainder >= (squaredbit | root)) {
			remainder -= (squaredbit | root);
			root >>= 1; root |= squaredbit;
		}
		else {
			root >>= 1;
		}
		squaredbit >>= 2;
	}

	return root;
}
bool IsSquare(long n)
{
	if (n < 0)
		return false;

	long tst = (long)(sqrt(n) + 0.5);
	return (tst*tst == n);
}

int gll(uni P, uni Q, int a, uni n) {
	if (P[4] == 0&&P[5] == 0){
		for (int i = 0; i < 6; i++)
			P[i] = Q[i];
		return 1;
	}
	if ( Q[5] == 0 && Q[4] == 0 ){
		return 1;
	}
	if ((P[0] == Q[0])&&(P[1]==Q[1])){
		if ((P[2] != Q[2])||(P[3]!=Q[3])){
			P[0]=1;
			P[1]=0;
			return 1;
		}
		if ((P[0] == 0)&&(P[1] == 0)){
			P[2]=1;
			P[3]=0;
			return 1;
		}
		else{
			uni_t t0[2]={2,0};
			uni_t t9[4]={3,0};
			uni_t t10[2]={a,0};
			uni_t t11[2];
			uni_t t1[4];
			uni_t t2[4];
			uni_t t3[4];
			uni_t t4[2];
			uni_t t5[2];
			uni_t t6[4];
			uni_t t7[4];
			uni_t t8[2];
			uni_t P0[2]={P[0],P[1]};
			uni_t P1[2]={P[2],P[3]};
			mult(t1,t0,P0,n);
			mult(t2,P0,P0,n);
			mult(t3,t9,t2,n);
			min_gcdx_lehmer(t11,t1,2, n,2);
			if ((t11[0] != 1)&&(t11[1] == 0)){
				P[4] = t1[0];
				P[5] = t1[1];
				return 1;
			}
			add(t4,t10,t3);
			mod_inv(t5,t1,n);
			mult(t6,t4,t5,n);
			mult(t7,t6,t6,n);
			unsigned long x[2];
			sub(x,t7,t1);
			sub(t8,P0,x);
			mult(t9,t8,t6,n);
			unsigned long y[2];
			sub(y,t9,P1);
			P[0] = x[0];
			P[1] = x[1];
			P[2] = y[0];
			P[3] = y[1];
			P[4] = 1;
			P[5] = 0;
			return 1;
		}
	}
	else{
		uni_t t1[2];
		uni_t t2[2];
		uni_t t3[2];
		uni_t t4[2];
		uni_t t5[4];
		uni_t t6[4];
		uni_t t7[2];
		uni_t t8[4];
		uni_t t9[2];
		uni_t P0[2]={P[0],P[1]};
		uni_t P1[2]={P[2],P[3]};
		uni_t Q0[2]={Q[0],Q[1]};
		uni_t Q1[2]={Q[2],Q[3]};
		sub(t1,Q0,P0);
		min_gcdx_lehmer(t9,t1,2, n,2);
		if ((t9[0] != 1)&&(t9[1] == 0)){
			P[4] = t1[0];
			P[5] = t1[1];
			return 1;
		}
		sub(t2,P1,Q1);
		sub(t3,P0,Q0);
		mod_inv(t4,t3,n);
		mult(t5,t2,t4,n);
		mult(t6,t5,t5,n);
		unsigned long x[2];
		sub(x,t6, t1);
		sub(t7,P0 , x);
		mult(t8,t5,t7,n);
		unsigned long y[2];
		sub(y,t8,P1);
		P[0] = x[0];
		P[1] = x[1];
		P[2] = y[0];
		P[3] = y[1];
		P[4] = 1;
		P[5] = 0;
		return 1;
	}

}

void mult2l(int b1, uni P, int a, uni n) {
	unsigned long result[6] = { 0,0, 1,0,0, 0 };
	for (int b = 2; b < b1; b++){
		while (b != 0){
			if ((b % 2) == 1){
				gll(P, result, a, n);
			}
			b = b / 2;
			gll(P, P, a, n);
			if ((P[2] > 1) || (P[2] == 0))
				break;
		}
		if ((P[2] > 1) || (P[2] == 0))
			break;
	}
}
unsigned long ecm(uni n, int bounda, int boundb) {
	unsigned long y2;
	unsigned long Bp = 100;
	for (int a = 1; a <= bounda; a++){
		if ((4*a*a*a + 27) == 0)
			continue;
		for (int b = 1; b <= boundb; b++){
			unsigned long x = 0;
			do{
				y2 = x*x*x + (a*x) + b;
				//printf("%d\n",y2);
				x = x + 1;
			} while (!IsSquare(y2)&& x<25);
			if (x>=25){ continue; }
			uni_t fpoint[6] = { x,0, Isqrt(y2),0, 1,0 };
			mult2l(Bp, fpoint, a, n);
			if (fpoint[2] == 0)
				break;
			uni_t g[2];
			min_gcdx_lehmer(g,fpoint,2, n,2);
			if (g[0] > 1){
				return g[0];
			}
		}
	}
	return 0;
}
int main() {
	srand((unsigned)time(NULL));
	uni_t n[2];
	for (int i = 0; i < 1; i++){
		n[0] = rand();
		n[1] = rand();
		printf("%lu\n", ecm(n, 5, 5));
	}
}
