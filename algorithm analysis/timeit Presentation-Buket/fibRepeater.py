from timeit import Timer

#Iterative method
def fibIter(n):
    if n<=2:
        return 1
    else:
        a,b,i=1,1,3
        while i<=n:
            a,b=b,a+b
            i+=1        
    return b

    
    
# Recursive method   
def fibRec(n):
    if n<=2:
        return 1
    else:
        return fibRec(n-2)+fibRec(n-1)

    
n=int(input("Enter a number :"))
s1 = fibIter(n)
s2 = fibRec(n)
#print(n,".Fibonacci number (iterative function)=",s1)
t1 = Timer("fibIter(n)","from __main__ import fibIter,n")
#print("Time of Iterative method =",t1.repeat(3))
#print(n,".Fibonacci number (recursive function)=",s2)
t2 = Timer("fibRec(n)","from __main__ import fibRec,n")
#print("Time of Recursive =",t2.repeat(3))



print("Time of Iterative method =",min(t1.repeat(3)))
print("Time of Recursive =",min(t2.repeat(3)))






