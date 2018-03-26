from sys import argv
from random import choice, randint


def select(a, k):
    v = choice(a)
    
    
    s_l = []
    s_v = []
    s_r = []

    for num in a:
        if(num < v):
            s_l.append(num)
        elif(num == v):
            s_v.append(num)
        else:
            s_r.append(num)


    if(k <= len(s_l)):
        return select(s_l, k)
    elif(k > len(s_l) and \
            k <= (len(s_l) + len(s_v))):
        return v
    else:
        k_adj = k - len(s_l) - len(s_v)
        return select(s_r, k_adj)
    




def main():

    n = int(argv[1])
    k = int(argv[2])

    print(argv[0], n, k)

    arr = []
    for rand_gen in range(0, n):
        rand_num = randint(0, n-1)
        arr.append(rand_num)

    kth_smallest = select(arr, k)
    print("select", kth_smallest)
    print("array", arr)

    arr.sort()

    print("sorted array", arr)

if __name__ == '__main__':
    main()
