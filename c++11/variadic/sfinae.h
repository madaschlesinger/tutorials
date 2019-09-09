#pragma once
#include <iostream>
#include <type_traits>


namespace detail
{
	template <typename T>
	struct C
	{
		void f(T const & t) { std::cerr << "f(ref)" << std::endl; }
        
        template< typename T_=T, typename std::enable_if_t< std::is_reference<T_>::value>* NotUsed=nullptr >
		void f(T_&& t)
        { std::cerr << "f(mv)" << std::endl; }
	};
};

void _sfinae() 
{

	std::cerr << "sfinae(success) : parts of actual run is at compile time." << std::endl ; 
 
	using detail::C;
	C<int> c;
	C<int&> cc;

	int a=10,b=100;
    int && rvalref = std::move( b );
	c.f(a);
	c.f(rvalref); // will cause compiler error w/o enable_ifcc.f(static_cast<int&>(a)); // will cause compiler error w/o enable_if
    cc.f(a); // will cause compiler error w/o enable_ifcc.f(static_cast<int&>(a)); // will cause compiler error w/o enable_if
    
    c.f(std::move( a ));       // creates rvalue reference
    //cc.f(std::move( a ) );    // creates rvalue reference -> compile error, b/c T is a reference - the function is sfinae'd away
	//static_assert(gcd<105,30>::value == 15, "must be 15");
}

