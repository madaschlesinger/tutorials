#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <typeinfo>
#include <functional>


using std::function ; 
using namespace std ; 




template<size_t... Ns>
struct Indexes
{};

template<typename... Ts, size_t...Ns>
auto cherry_pick(const tuple<Ts...>& t, Indexes<Ns...> ) -> decltype(make_tuple(get<Ns>(t)...))
{
  return make_tuple(get<Ns>(t)... ) ; 
}


void _cherry_pick() 
{
  std::cerr << "_cherry_pick() : " << std::endl ; 
  auto data = make_tuple(10,120123, "B737", "Boeing 737", 2, 1235000) ; 
  Indexes<0,2,4> even_indxes ;
  auto d = make_tuple(get<2>(data)) ; 
  cerr << get<0>(d) << endl;
  auto even_data = cherry_pick(data, even_indxes) ;
  cerr << get<0>(even_data) << endl;
}





