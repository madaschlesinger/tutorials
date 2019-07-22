import unittest
from string_utils import is_palindromic, int_to_string, atoi, convert_base_limited




class StringTests( unittest.TestCase ) :


	def test_palindromic(self ) :

		s = 'aba'
		self.assertTrue(is_palindromic(s))

		s = 'abc'
		self.assertFalse(is_palindromic(s))



	def test_int_to_string(self ) :

		self.assertEqual(int_to_string(123), '123' )
		self.assertEqual(int_to_string(1), '1')


	def test_atoi(self ) :

		self.assertEqual(atoi('123'), 123 )
		self.assertEqual(atoi('0'), 0)
		self.assertEqual(atoi('3456'), 3456)



	def test_convert_base_limited(self):

		r = convert_base_limited(10, 10, 10)  # should recover 10
		self.assertEqual(convert_base_limited(1, 10,  10),1)
		self.assertEqual(convert_base_limited(10, 10, 10),10)
		self.assertEqual(convert_base_limited(22, 10, 10),22)

		self.assertEqual(convert_base_limited(3, 10, 3),10)
		self.assertEqual(convert_base_limited(4, 10, 3),11)
		self.assertEqual(convert_base_limited(50, 10,7),101)

if __name__ == '__main__' :
	unittest.main()
	pass