#pragma once;
#include <iostream>


namespace detail
{
	template <int64_t N> struct fibonacci 
	{
		static constexpr int64_t value = fibonacci<N-1>::value + fibonacci<N - 2>::value;
	};

	template <> struct fibonacci<1>
	{
		static constexpr int64_t value = 1;
	};
	template <> struct fibonacci<0>
	{
		static constexpr int64_t value = 0;
	};


}
void _fibonacci() 
{

	std::cerr << "fibonacci(success) : actual run is at compile time." << std::endl ; 
 
	using detail::fibonacci;
	
	static_assert(fibonacci<0>::value == 0, "must be 0");
	static_assert(fibonacci<1>::value == 1, "must be 1");
	static_assert(fibonacci<2>::value == 1, "must be 1");
	static_assert(fibonacci<3>::value == 2, "must be 2");
	static_assert(fibonacci<4>::value == 3, "must be 3");
	static_assert(fibonacci<10>::value == 55, "must be 55");
}

