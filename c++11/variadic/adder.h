#pragma once

#include <string>
#include <iostream>
#include <cassert>



template< typename T>
T adder(T value)
{
	return value;
}

template< typename T, typename... Values>
T adder(T value, Values... vals)
{
	std::cerr << __FUNCSIG__ << std::endl;
	return value + adder(vals...);
}



void _adder()
{
	int i = adder(1, 2, 3, 10);
	assert(adder(1, 2, 3, 10) == 16);
}


