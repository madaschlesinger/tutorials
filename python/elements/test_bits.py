import unittest
from bits import parity, swap_bits, reverse_number, is_palindrome_number, uniform_random




class Parity( unittest.TestCase ) :



	def test_parity_odd(self ) :
		self.assertTrue(parity(1))
		self.assertTrue(parity(2))
		self.assertTrue(parity(4))
		# three bits set
		self.assertTrue(parity(7))

	def test_parity_even(self ) :
		self.assertFalse(parity(0))
		self.assertFalse(parity(3))
		# two bits
		self.assertFalse(parity(5))


	def test_swap_bits(self ) :

		i = 2
		j = 3
		x = 11        # b1011
		expected = 7  # b0111

		self.assertTrue(swap_bits(x,i,j) == expected)


	def test_reverse_number(self ) :

		self.assertTrue(reverse_number( 24 ) == 42 )
		self.assertTrue(reverse_number( -24) == -42)
		self.assertTrue(reverse_number( 513) == 315)

	def test_is_palindrome_number(self ) :

		self.assertTrue(is_palindrome_number( 1221) )
		self.assertFalse(is_palindrome_number( 1234) )

	def test_uniform_random(self ) :

		for i in range( 100 ) :

			r01 = uniform_random(0,1)
			r02 = uniform_random(0,2)
			r03 = uniform_random(0,3)
			r13 = uniform_random(1,3)
			r14 = uniform_random(1,4)
			r17 = uniform_random(1,7)
			r18 = uniform_random(1,8)

			self.assertTrue(r01 >= 0 and r01<= 1 )
			self.assertTrue(r02 >= 0 and r02<= 2 )
			self.assertTrue(r03 >= 0 and r03<= 3 )

			self.assertTrue(r13 >= 1 and r13<= 3 )
			self.assertTrue(r14 >= 1 and r14<= 4 )
			self.assertTrue(r17 >= 1 and r17<= 7 )
			self.assertTrue(r18 >= 1 and r18<= 8 )

		pass


	def test_itersects(self ) :

		from bits import Rect, intersect_rectangle

		square1 = Rect(0,0,2,2)
		square2 = Rect(0,0,1,1)
		square3 = Rect(1,1,1,1)
		square4 = Rect(1,1,3,3)
		square5 = Rect(5,5,1,1)

		intersects, r = intersect_rectangle(square1,square2)
		self.assertTrue(intersects)
		self.assertTrue( r.x == square2.x and r.y==square2.y )


		intersects, r = intersect_rectangle(square2,square1)
		self.assertTrue(intersects)
		self.assertTrue( r.x == square2.x and r.y==square2.y )

		# reverse order - intersect should be the same
		intersects, r = intersect_rectangle(square2,square1)
		self.assertTrue(intersects)
		self.assertTrue( r.x == square2.x and r.y==square2.y )


		# origin is different
		intersects, r = intersect_rectangle(square3,square1)
		self.assertTrue(intersects)
		self.assertTrue( r.x == square3.x and r.y==square3.y and r.width == 1 )

		intersects, r = intersect_rectangle(square1, square3)
		self.assertTrue(intersects)
		self.assertTrue(r.x == square3.x and r.y == square3.y and r.width == 1)

		# no overlap
		intersects, r = intersect_rectangle(square5, square1)
		self.assertFalse(intersects)


		# origin is different and sizes are different
		intersects, r = intersect_rectangle(square4,square1)
		self.assertTrue(intersects)
		self.assertTrue( r.x == square4.x and r.y==square4.y and r.width == 1 )

		intersects, r = intersect_rectangle(square1, square4)
		self.assertTrue(intersects)
		self.assertTrue(r.x == square4.x and r.y == square4.y and r.width == 1)



if __name__ == '__main__' :
	unittest.main()
	pass