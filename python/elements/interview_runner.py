import unittest
from   test_simple import Simple
from   test_bits import Parity
from   test_arrays import ArrayTests
from   test_strings import StringTests
from   test_linkedlist import LinkedListTests
from   tests.binary_tree import BinaryTreeTests


from bits import parity

if __name__ == '__main__' :
    result = parity(4)
    unittest.main()
    pass