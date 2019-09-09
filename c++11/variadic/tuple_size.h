#pragma once
#include <iostream>


/***************************************************
 *
 * tuple size example 
 *
 **************************************************/

// Determine memory size of a tuple at compile time
template<typename... Types> // allow zero parameters
struct TupleSize ; 


template<typename Head, typename... Tail> // standard, traverse types
struct TupleSize<Head,Tail...>
{
  static constexpr size_t value = sizeof(Head) + TupleSize<Tail...>::value ;
};


template<> struct TupleSize<> // ends recursion, with zero parameters
{
	static constexpr size_t value = 0;
};



namespace detail
{
	template <typename... Ts> struct tuple {};

	template <typename T, typename... Ts >
	struct tuple<T, Ts...> : tuple<Ts...>
	{
		tuple(T t, Ts... ts) : tail(t), tuple<Ts...>(ts...) {}

		T tail;
	};



}
void _tuple_size() 
{

	std::cerr << "_tuple_size(success) : actual run is at compile time." << std::endl ; 
 
	static_assert(TupleSize<>::value == 0, "must be zero");
	static_assert(TupleSize<int>::value == sizeof(int), "must be 4");
	static_assert(TupleSize<long>::value == sizeof(long), "must be 4");
	static_assert(TupleSize<float>::value ==4, "must be 4");
	static_assert(TupleSize<double>::value == 8, "must be 8");
	static_assert(TupleSize<int, double, char>::value ==13, "must be 13");


	detail::tuple<int, double, std::string> abc( 1,3.14, "pi");

}

