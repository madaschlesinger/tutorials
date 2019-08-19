#pragma once;
#include <iostream>


namespace detail
{
	template <int64_t N> struct factorial 
	{
		static constexpr int64_t value = N * factorial<N - 1>::value;
	};

	template <> struct factorial<0>
	{
		static constexpr int64_t value = 1;
	};



}
void _factorial() 
{

	std::cerr << "_factorial(success) : actual run is at compile time." << std::endl ; 
 
	using detail::factorial;
	
	static_assert(factorial<0>::value ==1, "must be 1");
	static_assert(factorial<1>::value == 1, "must be 1");
	static_assert(factorial<2>::value == 2, "must be 1");
	static_assert(factorial<3>::value == 6, "must be 6");
	static_assert(factorial<4>::value == 24, "must be 24");

}

