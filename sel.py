def sel1(l):
    for i in range(len(l)-1):
        k=i
        for j in range(i+1,len(l)):
            if l[j]<l[k]:
                k=j
        if k!=i:
            l[i],l[k]=l[k],l[i]
        
    return l
def sel2(l):
    for i in range(len(l)-1):
        k=i
        for j in range(i+1,len(l)):
            if l[j]<l[k]:
                k=j
        l[i],l[k]=l[k],l[i]
        
    return l
def sel1Count(l):
    ass=0
    com=0
    for i in range(len(l)-1):
        k=i
        for j in range(i+1,len(l)):
            com=com+1
            if l[j]<l[k]:
                k=j
        com=com+1
        if k!=i:
            l[i],l[k]=l[k],l[i]
            ass=ass+2
    return l,ass,com
def sel2Count(l):
    ass=0
    com=0
    for i in range(len(l)-1):
        k=i
        for j in range(i+1,len(l)):
            com=com+1
            if l[j]<l[k]:
                k=j
        l[i],l[k]=l[k],l[i]
        ass=ass+2
    return l,ass,com
