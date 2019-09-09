//
//  main.cpp
//  variadic
//
//  Created by Adam Schlesinger on 8/19/19.
//  Copyright © 2019 Adam Schlesinger. All rights reserved.
//


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
#include "initializer_list.h"
#include "codility.h"
#include "vector.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    // codility::brackets::solution();
    // codility::fish::solution();
//    codility::oldmission::solution();
     _vector();
    
#if 0
    _initializer_list();
    _sfinae();
    _gcd();
    _fibonacci();
    _factorial();
    _printer()  ;
    _tuple_size()  ;
    _cherry_pick()  ;
    _adder();
#endif
    return 0;
}
