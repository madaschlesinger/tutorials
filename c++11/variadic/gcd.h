#pragma once;
#include <iostream>


namespace detail
{
	template <int64_t M, int64_t N> struct gcd // M > N
	{
		static constexpr int64_t value = gcd<N, M%N>::value ;
	};
	template <int64_t M> struct gcd<M,0>
	{
		static constexpr int64_t value = M;
	};

}
void _gcd() 
{

	std::cerr << "gcd(success) : actual run is at compile time." << std::endl ; 
 
	using detail::gcd;
	
	static_assert(gcd<105,30>::value == 15, "must be 15");
}

