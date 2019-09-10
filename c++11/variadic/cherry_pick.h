#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <typeinfo>
#include <functional>


using std::function ; 


template<size_t... Ns>
struct Indexes
{};

template<typename... Ts, size_t...Ns>
auto cherry_pick(const std::tuple<Ts...>& t, Indexes<Ns...> ) -> decltype(std::make_tuple(std::get<Ns>(t)...))
{
    return std::make_tuple(std::get<Ns>(t)... ) ;
}


void _cherry_pick() 
{
  std::cerr << "_cherry_pick() : " << std::endl ; 
  auto data = std::make_tuple(10,120123, "B737", "Boeing 737", 2, 1235000) ;
  Indexes<0,2,4> even_indxes ;
  auto d = std::make_tuple(std::get<2>(data)) ;
  std::cerr << std::get<0>(d) << std::endl;
  auto even_data = cherry_pick(data, even_indxes) ;
  std::cerr << std::get<0>(even_data) << std::endl;
}





