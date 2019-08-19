#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <typeinfo>
#include <functional>

#include "tuple_size.h"
#include "cherry_pick.h"
#include "printer.h"
#include "adder.h"
#include "factorial.h"
#include "fibonacci.h"
#include "gcd.h"
#include "sfinae.h"


using std::function ; 
using namespace std ; 



int main() 
{
	_sfinae();
	_gcd();
	_fibonacci();
	_factorial();
	_printer()  ;
	_tuple_size()  ; 
	_cherry_pick()  ; 
	_adder();
	return 0 ;
}



