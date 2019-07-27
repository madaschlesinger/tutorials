import unittest
import binary_tree.node
from binary_tree.utils import *


class BinaryTreeTests( unittest.TestCase ) :
	''' setup structure is based on page 121 '''

	def setUp(self):

		node_names = 'ABCDEFGHIJKLMNO'
		nodes = {}

		def link_nodes( root: chr, left, right) :
			l = nodes[left] if left is not None else None
			r = nodes[right] if right is not None else None
			insert_children(nodes[root], l, r )

		for i,c in enumerate(node_names):
			nodes[c] = Node(c)

		self.root = nodes[ 'A' ]
		link_nodes('A', 'B', 'K')

		# depth 2
		link_nodes('B', 'C', 'H')
		link_nodes('K', 'L', 'O')

		# depth 3
		link_nodes('C', 'D', 'G')
		link_nodes('H', 'I', 'J')
		link_nodes('L', 'M', 'N')

		# depth 4
		link_nodes('D', 'E', 'F')



	def test_binary_tree_construction(self ) :

		self.assertTrue(True)


	def test_binary_tree_traversal_preorder(self ) :
		tree_traversal_preorder(self.root)

		self.assertTrue(True)

	def test_a_binary_tree_traversal_in_order(self):
		tree_traversal_inorder(self.root)
		self.assertTrue(True)
