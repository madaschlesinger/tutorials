


def parity(x: int) -> int:

    return parity_better(x)

def parity_original(x: int) -> int:
    result = 0
    while x:
        result ^= x & 1
        x >>= 1
        pass
    return result

def parity_better(x: int) -> int:
    result = 0
    while x:
        result ^= 1
        x &= (x-1)    # this shifts off the lowest set bit - so faster loop runs number of set bits install of all bits
        pass
    return result


def swap_bits(x, i, j):

    # check to see if the bits in the ith and j-th bits are different
    if( x >> i ) & 1 == (x >> j & 1 ) :
        return x

    bit_mask = (1 << i) | (1 << j)

    # xor will now cause them to flip - we know one is a zero and one is a one....
    # xor with a 1,1 in those bits will cause the zero to go to 1, the one to go to 0

    x ^= bit_mask

    return x


def reverse_number(x):
    '''24 -> 42, -513 -> -315'''

    result = 0
    x_remaining = abs(x)

    while x_remaining :
        last_digit = x_remaining % 10
        result = result * 10 + last_digit
        x_remaining //= 10

    return -result if x < 0 else result


def is_palindrome_number(x):
    '''is palidrome example 1221 - yes, 1234 - no'''

    import math

    num_digits = math.floor(math.log10(x)) +1
    msd_mask   = 10**(num_digits-1)

    for i in range( num_digits // 2 ) :
        msd = x // msd_mask
        lsd = x % 10

        if lsd != msd : return False

        x %= msd_mask
        x //= 10
        msd_mask //=100 # we took off two digits in each pass


    return True

def zero_one_random() :
    import random
    return random.randint(0,1)


def uniform_random(lower_bound: int, upper_bound):
    ''' random number generator - bounded w/only zero or one random number generator available'''

    import math

    r, n_outcomes = 0, upper_bound-lower_bound+1

    while True:

        result, i = 0,0

        while ( 1 << i ) < n_outcomes :

            result  = ( result << 1 ) |  zero_one_random()
            i += 1

        w = 1 << i

        if (result) < n_outcomes :
            break

    return result + lower_bound


import collections
Rect = collections.namedtuple( 'Rect', ('x', 'y', 'height', 'width'))

def intersect_rectangle( r1:Rect, r2:Rect) -> Rect :

    def does_intersect(r1: Rect, r2: Rect) -> bool :

        return r1.x + r1.width >= r2.x and r2.y + r2.height >= r1.y

    if not ( does_intersect(r1,r2) or does_intersect(r2,r1) ):
        return False, Rect(0,0,-1,-1)

    x  = max(r1.x,r2.x)
    y  = max(r1.y,r2.y)

    xprime = min( r1.x+r1.width , r2.x+r2.width)
    yprime = min(r1.y + r1.height, r2.y + r2.height)
    w  = xprime - x
    h = yprime -y
    r = Rect(x, y,w,h)

    return True, r

