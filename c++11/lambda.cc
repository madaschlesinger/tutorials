#include <stdio.h>
#include <iostream>
#include <vector>
#include <typeinfo>
#include <functional>

using std::function ; 

class LambdaStore
{
  function<bool(double)> _stored_lambda ; 
public:
  
  function<int(int)> get_abs() const 
  {
    return [](int i) { return abs(i) ; } ; 
  }

  void set_lambda( const function<bool(double)>& l)
  {
    _stored_lambda = l; 
  };

};


int func(double) { return 10 ; }

template<typename T>
int f(T t)
{
  return 3;
}

template<typename X, typename Y>
auto multiply(X x, Y y) -> decltype(x*y)
{
  return x*y ; 
}


function<bool()> gg()
{
  static int a = 5; 
  static int b = -3 ; 

  return []() { return a+b > 0 ; } ; 
}

function<bool()> g()
{
  int a = 5; 
  int b = -3 ; 

  return [a,b]() { return a+b > 0 ; } ; 
}


static void _simple(void) 
{
 
  auto f = [](int i) { return 43.2 * i; } ; 

  std::cerr << "simple() :  " << f(2) << std::endl; 

}

static void _stored() 
{
  LambdaStore ls ; 

  ls.set_lambda( [](double d) { return d>0.0 ; } ) ; 


  auto abs_lambda = ls.get_abs()  ; 

  std::cerr << "stored() : " << abs_lambda( -10 ) << std::endl; 

  return;
}

static void _fibonaci()
{
  std::cerr << "_fibonaci() : " ; 

  // recursive trick for lambdas - wrap in std::function
  function<int(int)> fibonaci = [&](int n) -> int
    {
      if(n<1)
	return -1 ; 
      else if (n==1 || n==2 )
	  return 1 ;
      else
	  return fibonaci( n-1 )+ fibonaci( n-2 ) ; 
    };

  std::cerr << "_fibonaci() : " ; for(auto i=1; i < 10; ++i ) std::cerr << fibonaci(i) << " ";  
  std::cerr << std::endl;
}


int main() 
{
  _simple() ; 
  _stored(); 
  _fibonaci() ; 
  return 0 ; 
}



