def ins(l):
    for i in range(1,len(l)):
        x=l[i]
        j=i-1
        while (j>=0 and l[j]>x):
            l[j+1]=l[j]
            j-=1
        l[j+1]=x
    return l
# this is the function without counters

def insCount(l):
    comp,ass=0,0
    for i in range(1,len(l)):
        x=l[i]
        ass+=1
        j=i-1
        comp+=1
        while (j>=0 and l[j]>x):
            l[j+1]=l[j]
            ass+=1
            j-=1
            if j>=0:
                comp+=1
        l[j+1]=x
        ass+=1

    print("Sorted list is : ",l)
    print("Number of comparisons is :",comp)
    print("Number of assignments is :",ass)

    n=len(l)
    a=(n-1)*(n+4)//2
    c=n*(n-1)//2
    cbest=n-1
    abest=2*cbest
    print("n is : ", n)
    print("Theoretically, comparisons in worst case = n(n-1)/2 = ",c)
    print("Theoretically, comparisons in best case = n-1 = ",cbest)
    print("Theoretically, assignments in worst case = (n+4)(n-1)/2 = ", a)
    print("Theoretically, assignments in best case = 2(n-1) = ", abest)

# counting both
