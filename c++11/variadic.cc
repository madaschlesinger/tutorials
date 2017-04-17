#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <typeinfo>
#include <functional>

using std::function ; 
using namespace std ; 

template< typename Stream, typename... Columns>
class Printer
{  

public:
  bool _firsttime=true ; 
  char _sep = ','; 
  array<string, sizeof...(Columns)> _headers ; // getting size of param packs for heards 
public:


  
  template<typename T>
  T validate( const T&    t ) { return t ; } // no validation....

  template<typename Value> 
  void write_column( const Value& value, const char sep )
  {
    cerr << value << sep ; 
  }

  // example of unrolling a parameter pack - template meta programming
  template<typename Value, typename... Values>
  void write_line( const Value& value, const Values&... values) 
  {
    if( _firsttime )
      {
	//write_line( _headers ); // dont have this working
	_firsttime = false ; 
      }
    write_column(value, _sep ) ; 
    write_line( values...    ) ;
  }


  // terminate recursion - and add newline
  template<typename Value>
  void write_line( const Value& value) 
  {
    write_column(value, '\n' ) ; 
  }


  void output_line( const Columns&... columns ) 
  {
    write_line( validate( columns )... ) ; 
  }

};

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
  static const size_t value = sizeof(Head) + TupleSize<Tail...>::value ; 
  
};

template<> struct TupleSize<> // ends recursion, with zero parameters
{
  static const size_t value = 0 ; 
};


/***************************************************
 *
 * tuple selection example - subsets
 *
 **************************************************/

template<size_t... Ns>
struct Indexes
{};

template<typename... Ts, size_t...Ns>
auto cherry_pick(const tuple<Ts...>& t, Indexes<Ns...> ) -> decltype(make_tuple(get<Ns>(t)...))
{
  return make_tuple(get<Ns>(t)... ) ; 
}



static void _cherry_pick() 
{
  std::cerr << "_cherry_pick() : " << std::endl ; 
  auto data = make_tuple(10,120123, "B737", "Boeing 737", 2, 1235000) ; 
  Indexes<0> i0 ;
  auto d = make_tuple(get<2>(data)) ; 
  cerr << get<0>(d) << endl;
  auto even = cherry_pick(data, i0) ; 
  cerr << get<0>(even) << endl;
}


static void _tuple_size() 
{
 
  std::cerr << "_tuple_size() : " << std::endl ; 
  std::cerr << TupleSize<>::value << endl;
  std::cerr << TupleSize<int>::value << endl;
  std::cerr << TupleSize<long>::value << endl;
  std::cerr << TupleSize<float>::value << endl;
  std::cerr << TupleSize<double>::value << endl;
  std::cerr << TupleSize<int,double,char>::value << endl;

}


static void _printer()
{
  std::cerr << "_printer() : " << std::endl ; 
  Printer< istream, int, double, string> printer ; 

  string mystr("foo_bar"); 

  printer._headers[0] ="int" ; 
  printer._headers[1] ="double" ; 
  printer._headers[2] ="string" ; 


  printer.output_line(2, 3.14, mystr ) ;
  printer.output_line(2, 3.14, "goo" ) ;
  printer.output_line(2, 3.14, "noo" ) ;


}


int main() 
{
  _printer()  ; 
  _tuple_size()  ; 
  _cherry_pick()  ; 
  return 0 ; 
}



