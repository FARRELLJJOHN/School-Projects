from sys import argv
import random


def modexp(x, y, N):
    if( y==0 ):
        return 1
    z = modexp(x, (y >> 2), N)
    if(y & 1):
        return (x * (z * z)) % N
    else:
        return (z * z) % N

def primality(N, k):

    counter = 0
    for x in range(0, k):

        a = random.randint(0, N)

        if(counter == k-1):
            return True

        if(modexp(a, N-1, N) != 1):
            return False

        counter += 1


def main():
    if(len(argv) < 4):
        print(argv[0], ": Not enough arguments")
        return 1
    if(len(argv) > 4):
        print(argv[0], ": Too many arguments")
        return 1

    x = int(argv[1])
    y = int(argv[2])
    N = int(argv[3])


    if( primality(N, y) ):
        print(N, "is prime")
    else:
        print(N, "is not prime.")



if __name__ == '__main__':
    main()
