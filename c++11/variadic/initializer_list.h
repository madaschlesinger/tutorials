#pragma once
#include <iostream>


namespace detail {
    
    template <typename T=int>
    struct X {
        
        std::vector<T> v;
        X(std::initializer_list<T> i) : v(i) {}
    };
}

void _initializer_list()
{
    using detail::X;
    
	std::cerr << "_initializer_list(success) : " << std::endl ;
 
    X<int> x = { 1, 2, 3 };

    assert( x.v.size() == 3 );
}

