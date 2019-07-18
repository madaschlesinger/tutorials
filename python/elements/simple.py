import unittest

def setUpModule() :
	pass

def tearDownModule() :
	pass


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
		self.assertTrue( True )


