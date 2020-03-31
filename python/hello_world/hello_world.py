
''' i would need to get the pytest stuff going...

to get it going - you need homebrew to start.  brew
is an essential tool.  Read on how to install homebrew on your mac

then i just ran

brew install pytest -

I will get modules / testing going later

from here however, you can learn to run a simlple program

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


