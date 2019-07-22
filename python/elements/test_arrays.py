import unittest
from array_utils import odd_even_partition, dutch_flag_partition, delete_dups, \
	stock_picker_single,\
	stock_picker_two,\
	primes,\
	pascals_triange




class ArrayTests( unittest.TestCase ) :



	def test_even_odd(self ) :

		l1 = [ 0,1,2,3,4,5,6,7]
		e1 = [ 0,6,2,4,5,3,7,1]

		odd_even_partition(l1)
		self.assertTrue(l1 == e1)



	def test_dutch_partition(self ) :

		l1 = [ 5,4,3,2,1]
		e1 = [1,2,3,4,5]

		dutch_flag_partition(2,l1)
		self.assertTrue(l1 == e1)

		l2 = [3, 5, 4, 3, 3, 3 ,2,1,-1,4,2, 1, 5, 5 ]
		e1 = [1, 2, 3, 4, 5]
		dutch_flag_partition(5, l2)
		pass


	def test_delete_dups(self):

		l1 = [ 1, 2, 2, 3, 4, 5, 6, 6, 7]
		e1 = [1,2,3,4,5,6,7]

		a = delete_dups(l1)

		self.assertTrue(a == e1)
		pass

	def test_stock_picker_single(self):

		px = [ 12,11,13,9,12,8,14,13,15]

		p1 = stock_picker_single(px)
		p2 = stock_picker_two(px)  # not quite right
		self.assertTrue(p1 == 7)

		pass


	def test_primes(self):

		p = primes(20)
		self.assertTrue(len(p) == 8)

		pass

	def test_pascal(self):

		p = pascals_triange(3)
		l=len(p)
		self.assertTrue(len(p) == 3)
		self.assertTrue( p[2][1] == 2)
		pass

if __name__ == '__main__' :
	unittest.main()
	pass