import unittest

def setUpModule() :
	pass

def tearDownModule() :
	pass


def tf() :
	return 1

class Simple( unittest.TestCase ) :


	def setUp( self ) :
		pass

	def tearDown( self ) :
		pass

	@classmethod
	def tearDownClass( cls ) :
		pass

	@classmethod
	def setUpClass( cls ) :
		pass


	def test_tf(self ) :
		rc = tf()
		self.assertTrue( True )


if __name__ == '__main__' :
	unittest.main()
	pass