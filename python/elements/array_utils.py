
def odd_even_partition(l):

    even_inserter , odd_inserter = 0, len(l) -1

    while even_inserter < odd_inserter:

        if l[even_inserter] % 2 == 0:
            even_inserter += 1
        else:
            l[even_inserter], l[odd_inserter] = l[odd_inserter], l[even_inserter]
            odd_inserter -= 1

    return None


def dutch_flag_partition( pivot_index:int, A ) -> None :

    smaller, equal, larger = 0, 0, len(A) -1
    pivot = A[pivot_index]


    while equal < larger :

        item = A[equal]
        if A[equal] < pivot :
            A[smaller], A[equal] = A[equal], A[smaller]
            equal, smaller = equal + 1, smaller + 1

        elif A[equal] == pivot :
            equal += 1
        else:
            A[larger], A[equal] = A[equal], A[larger]
            larger -= 1

        pass


def delete_dups( l ) -> int :
    ''' input must be sorted'''

    write_index = 1

    for i in range(1,len(l) ) :

        a = l[write_index-1]
        b = l[i]

        if l[write_index-1] != l[i]:
            l[write_index] = l[i]
            write_index += 1

    # l = l[:write_index] THIS DOES NOT WORK - NOT AN IN PLACE ASSIGNMENT
    return l[:write_index]


def stock_picker_single( prices ) -> int :
    ''' input array of stock prices'''

    max_profit, min_price  = 0, float('inf')
    profit_tracker = [0.0] * len( prices )

    for i, p in enumerate(prices):

        max_profit = max( max_profit, p - min_price )
        min_price  = p if p < min_price else min_price

        profit_tracker[i] = max_profit


        pass


    return max( profit_tracker )


def stock_picker_two( prices ) -> int :
    ''' not quite correct was tracking sell index - need to track buy'''

    import collections
    Tracker = collections.namedtuple('Tracker', ('index', 'profit'))

    def max_profit_calculator( prices ) -> Tracker:

        max_profit, min_price  = 0, float('inf')
        profit_tracker = [0.0] * len( prices )
        max_tracker    = Tracker(0,0)

        for i, p in enumerate(prices):

            max_profit = max( max_profit, p - min_price )
            min_price  = p if p < min_price else min_price

            max_tracker = Tracker(i, max_profit) if max_profit > max_tracker.profit else max_tracker

            pass

        return max_tracker

    p1 = max_profit_calculator(prices)
    p2 = max_profit_calculator(prices[:p1.index])

    return p1.profit + p2.profit




def primes( n:int) -> list:
    ''' prints prime numbers to n'''

    is_prime = [False, False] + [True] * (n-1)

    for p in range(2, n+1):

        if not is_prime[p]:
            continue

        for i in range(2*p, n+1, p):
            is_prime[i] = False

        pass

    primes = [i for i,p in enumerate(is_prime) if p is True ]

    return primes

def pascals_triange( n:int) -> list:
    ''' take in N - number of rows in the triangle'''

    r = [[1] * (i + 1) for i in range(n)]

    for i in range(n):
        for j in range(1, i ):
            r[i][j] = r[i - 1][j - 1] + r[i - 1][j]

    return r