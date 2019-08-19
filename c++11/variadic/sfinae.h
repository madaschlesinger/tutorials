#pragma once;
#include <iostream>


namespace detail
{
	template <typename T>
	struct C
	{
		void f(T const & t) { std::cerr << "f(ref)" << std::endl; }
		void f(T&& t) { std::cerr << "f(mv)" << std::endl; }
	};
};

void _sfinae() 
{

	std::cerr << "sfinae(success) : parts of actual run is at compile time." << std::endl ; 
 
	using detail::C;
	C<int> c;
	C<int&> cc;

	int a;
	c.f(a);
	cc.f(static_cast<int&>(a)); // will cause compiler error w/o enable_if

	//static_assert(gcd<105,30>::value == 15, "must be 15");
}

