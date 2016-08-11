count=0
def fibRec( n ):
    global count
    if n <=2 : 
        return 1
    else:
        count+=1
        return fibRec( n - 1 ) + fibRec( n - 2 )
        


def checkFib():
    global count
    number = int( input( "Enter an integer: " ) )

    if number > 0:
        result = fibRec( number )
        print ("Fibonacci(%d) = %d" % ( number, result ))
        print ("Number of additions is=" ,count)
    else:
        print ("Cannot find the fibonacci of a negative number")
    count=0
        
    
def checkFib2(number):
    global count
    if number > 0:
        result = fibRec( number )
        print ("Fibonacci(%d) = %d" % ( number, result ))
        print ("Number of additions is=" ,count)
    else:
        print ("Cannot find the fibonacci of a negative number")
    count=0
        
   
