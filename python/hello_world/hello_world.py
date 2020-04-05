
'''

Tests - note the test dir.  There are two basics test frameworks for python
unittest - and pytest.  I used unittest here b/c it worked out of the box.

On the upper right ( in my cfg ) - in the menu bar you can see a drop down for
configurations.  There are two defined - UnitTest in test_foo.py - and hello_world.


note the 'hello_world' in the configuration - upper right on menu bar
the triangle to the left - is to run the program ( here there is only one )
the 'bug' to the right is to debug the program.  The debugging window on
the lower left will have icons to step over ( exceute next line ) - step into
step outof.  Read how to set break points etc.

'''

from hw.foo import bar


def times_two ( x ) :
	return x * 2



def main() :
	x = bar(4)
	y = times_two( x )
	print( "hello world 2*x^2 = %d!" % y)



if __name__ == "__main__" :
	main()


