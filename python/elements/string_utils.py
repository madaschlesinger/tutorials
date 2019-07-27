

def is_palindromic( s:str) -> bool :
    x=len(s) // 2
    r = all(s[i]==s[~i] for i in range( len(s) // 2 ) )
    return r

def int_to_string(x: int):

    s=[]

    while True:

        s.append( chr( ord('0')+x%10 ))
        x //= 10

        if x == 0 :
            break

    return ''.join(reversed(s))


def atoi(s: str) -> int:

    import string
    result = 0
    for i in range( len(s) ):
        c = s[i]
        result = result * 10 + string.digits.index(c)

    return result

def convert_base_limited( x:int, b:int, base:int):
    '''
        works only partially as written - i.e. all can be bases must be int - cant be base 13 as an example
        that would require strings/letter etc.

        First convert to base 10 - then from base 10 -> new base
    '''

    import math

    n=x
    result_b10=0
    for i in range(len(str(x))):
        lsd = n%10
        result_b10 = result_b10 + b**i * lsd
        n //=10



    power = 0
    while True:
        if (base-1)*base**power >= result_b10:
            break
        power += 1

    rebased = 0 ;
    r = result_b10
    # now convert b10 -> new base
    while power >= 0:

        d = r // base ** power
        r -= d*base**power
        rebased = rebased * 10 + d
        power -= 1

    return rebased


def spread_sheet_column_to_int( column:str) -> int :
    ''' return zero one based column name to column number conversion '''
    key = '~ABCDEFGHIJKLMNOPQRSTUVWXYZ'

    result=0
    for c in column :
        result = result * 26 + key.index(c)

    return result




