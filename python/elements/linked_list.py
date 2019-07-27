
import binary_tree.node

class ListNode :

    def __init__(self, data=0, nxt=None):
        self.data = data
        self.next = nxt

def search_list(l: ListNode, key:int) -> ListNode :

    while l and l.data !=key:
       l = l.next

    return l


def append_node( l: ListNode, n:ListNode) -> ListNode :

    ''' wind L to end ( or you could pass the end '''
    while l :
       l = l.next

    l.next = n

    return n


def insert_node( n: ListNode, nn:ListNode) -> ListNode :

    nn.next = n.next
    n.next  = nn

    return nn


def delete_following_node(n: ListNode ) -> ListNode:
    ''' needs to be doubly linked to just have delete_node '''

    n.next = n.next.next

    return n