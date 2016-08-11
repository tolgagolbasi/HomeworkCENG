def foo(n):
    q=0
    for j in range(n):
        q=q+(j*j)+j
    return q
def foo2(n):
    return (n*(n+1)/2)+(n*(n+1)*(2*n+1)/6)-n*n-n
