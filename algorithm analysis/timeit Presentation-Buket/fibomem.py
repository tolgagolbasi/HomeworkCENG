from timeit import Timer

memo = {0:0, 1:1}
def fibm(n):
    if not n in memo:
        memo[n] = fibm(n-1) + fibm(n-2)
    return memo[n]


def fibi(n):
    a, b = 0, 1
    for i in range(n):
        a, b = b, a + b
    return a



for i in range(1,41):
	s = "fibm(" + str(i) + ")"
	t1 = Timer(s,"from __main__ import fibm")
	time1 = t1.timeit(3)
	s = "fibi(" + str(i) + ")"
	t2 = Timer(s,"from __main__ import fibi")
	time2 = t2.timeit(3)
	print("n=%2d, fibMemory: %8.6f, fibIterative:  %7.6f" % (i, time1, time2))

