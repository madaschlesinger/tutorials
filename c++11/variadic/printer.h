#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <typeinfo>
#include <functional>
#include <array>

#include "tuple_size.h"
#include "cherry_pick.h"




using std::function ; 
using namespace std ; 

#if 1
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

#endif


void _printer()
{
	Printer<decltype(std::cerr), int, double, string > printer;
	printer.output_line(1, 3.14, "pi");
}


