clear;
tester := function(algorithm, nbits, trial)
	t := Cputime();
	for i:=1 to trial do
		N:=Random([5..(2^nbits)]);
		p:=algorithm(N);
	end for;
	return Cputime(t)/trial;
end function;
FormFactorBase := function(f)
	fb := [];
	i:=1;
	while i lt f do
		i := NextPrime(i);
		fb := Append(fb, i);
	end while;
	return fb;
end function;

SqrtIntegerPart := function(n)
	x := 2^Ceiling(Log(2,n)/2);
	y := Floor((x+Floor(n/x))/2);
	while y lt x do
		x := y;
		y := Floor((x+Floor(n/x))/2);
	end while;
	return x;
end function;

ExponentVector := function(k, s, fb)
	W:=[]; // integer
	V:=[]; // mod 2
	V[1] := GF(2)!s;
	W[1] := s;
	for i:=1 to #fb do
		j := 0;
		while IsDivisibleBy(k, fb[i]) do
			k := k div fb[i];
			j := j+1;
		end while;
		V := Append(V, GF(2)!j);
		W := Append(W, j);
	end for;
	V := Append(V, GF(2)!0);
	W := Append(W, 0);
	if k eq 1 then 
		return V, W, true;
	else
		return V, W, false;
	end if;	
end function;

SqrtContinuedFractionsExpansion := function(N, k)
	AN := [];
	QN := [];
	QS := [];
	P0 := 0; Q0 := 1; 
	b_zero := SqrtIntegerPart(N);
	b0 := b_zero;
	An2 := 1; An1 := b_zero;

	i:=0;
	while i lt k do
		P1 := Q0*b0 - P0;
		Q1 := (N - P1^2) div Q0; 
		b1 := (b_zero + P1) div Q1;
		An := b1*An1 + An2;
		
		// print [[An2, Q0], [(An2^2) mod N, (Q0*(-1)^i) mod N]];
		AN := Append(AN, An2 mod N);
		QN := Append(QN, Q0);
		QS := Append(QS, i);

		P0 := P1; Q0 := Q1; b0 := b1;		
		An2 := An1; An1 := An;

		i := i+1;
	end while;
	return AN, QN, QS;
end function;

FactorCongruentSquares := function(n)
	s := SqrtIntegerPart(n);
	f := SqrtIntegerPart(s);
	fb := FormFactorBase(f);
      
	AN, QN, QS := SqrtContinuedFractionsExpansion(n, f);

	// print "AN := ", AN, "QN := ", QN;
	//print "fb:", fb;
	MAN := [];
	MV := [];
	MW := [];
	i := 1; 

	while (i lt #QN+1) do
		V, W, ck := ExponentVector(QN[i], QS[i], fb);
		if ck eq true then
			MV := Append(MV, V);
			MW := Append(MW, W);
			MAN := Append(MAN, AN[i]);
		end if;
		i := i+1;
	end while;
	X := Matrix(GF(2), #MV, #fb + 2, MV);
	N := Nullspace(X);
	N := Basis(N);

	//print "N:", N;
	//print "MW:", MW;

	
	for i:=1 to #N do
		//print "N'deki denenen çözüm numarası:", i;
		T := []; R := 1;
		for k:=1 to #fb do
			T[k] := 0;
		end for;
		for j:=1 to #MV do
			if N[i][j] eq 1 then
				for k:=1 to #fb do
					T[k] := T[k] + MW[j][k+1];
				end for;
				R := R * MAN[j];
			end if;
		end for;
		for k:=1 to #fb do
			T[k] := T[k] div 2;
		end for;
		
		P := 1;
		for j:=1 to #fb do
			P := P * (fb[j]^T[j]);
			//print "fb[j] := ", fb[j];
			//print "T[j] := ", T[j];
		end for;
		
		//print [P^2 mod n, R^2 mod n];
		
		if IsDivisibleBy(n, GCD(P+R, n)) and GCD(P+R, n) ne 1 and GCD(P+R, n) ne n then
			return GCD(P+R, n);
		end if;
		
		if IsDivisibleBy(n, GCD(P-R, n)) and GCD(P-R, n) ne 1 and GCD(P-R, n) ne n then
			return GCD(P-R, n);
		end if;
	end for;
	return N,MW,MAN;
end function;

for i:=30 to 40 do
	print i, tester(FactorCongruentSquares, i, 10);
end for;
