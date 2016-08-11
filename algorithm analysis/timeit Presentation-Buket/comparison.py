from timeit import Timer
def fibRec(n):
    if n <=2:
        return 1    
    else:
        return fibRec(n-1) + fibRec(n-2)


def fibIter(n):
    if n<=2:
        return 1
    else:
        a,b,i=1,1,3
        while i<=n:
            a,b=b,a+b
            i+=1           
    return b



for i in range(1,41):
	s1 = "fibRec(" + str(i) + ")"
	t1 = Timer(s1,"from __main__ import fibRec")
	time1 = t1.timeit(1)
	s2 = "fibIter(" + str(i) + ")"
	t2 = Timer(s2,"from __main__ import fibIter")
	time2 = t2.timeit(1)
	print("n=%2d,fib%2d=%12d, fibRec: %9.6f, fibIter: %7.6f" % (i,i,fibIter(i), time1, time2))
