
from binary_tree.node import Node



def insert_children( root: Node, left: Node, right : Node ) -> None :

    if root:
        root.left = left
        root.right = right

def add_left( root: Node, left: Node ) -> None :

    if root:
        root.left = left

def add_right( root: Node, right: Node ) -> None :

    if root:
        root.right = right


def tree_traversal_preorder( root: Node ) -> None :
    '''pre-order you traverse left root then right'''

    if root:
        print( 'Pre-order: %s' % root.data )
        tree_traversal_preorder(root.left)
        tree_traversal_preorder(root.right)


def tree_traversal_inorder( root: Node ) -> None :
    '''pre-order you traverse left root then right'''

    if root:
        tree_traversal_inorder( root.left)
        print( 'In-order: %s' % root.data )
        tree_traversal_inorder( root.right)


def tree_traversal_postorder( root: Node ) -> None :
    '''pre-order you traverse left, right then root'''

    if root:
        tree_traversal_postorder(root.left)
        tree_traversal_postorder(root.right)
        print( 'Post-order: %s' % root.data )