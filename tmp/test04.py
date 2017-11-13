if __name__ == '__main__':
    n = int(raw_input())
    x = n % 2

    print x
    print n

    if x != 0:
        print "Weird"
    else:
        if n > 1 and n < 6:
            print "Not Weird" 
        else:
            if n > 6 and n < 20:
                print "Weird"
            else:
                if n > 20: 
                    print "Not Weird"
