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

function<bool()> ggg()
{
  int a = 5; 
  int b = -3 ; 

  return [a,b]() { return a+b > 0 ; } ; 
}


void myPrintHelloMake(void) {

  int a = 0 ; 
  auto b = a; 

  double fleet[10];

  std::vector<int> values ;
  auto  aa  =  values ; 
  auto& bb  =  values ; 
  auto *ptr = &values ;
  
  values.push_back( 1 ) ; 
  values.push_back( 1 ) ; 

  auto& g=func ; 

  printf("Hello makefiles : %d !\n", f(3.4) );

  int i = 10 ; 

  decltype(i) bbb; 

  std::cerr << "wow...wtf " << typeid(decltype(i+1.0)).name() << std::endl;
  std::cerr << "wow...wtf " << multiply(3,4) << std::endl; 


  auto plane = fleet ;

  *plane = 100 ; 

  auto ff = [](int i) { return 43.2 * i; } ; 

  std::cerr << "wow...wtf " << ff(2) << std::endl; 

  
  LambdaStore ls ; 

  ls.set_lambda( [](double d) { return d>0.0 ; } ) ; 


  auto abs_lambda = ls.get_abs()  ; 

  std::cerr << "wow...wtf " << abs_lambda( -10 ) << std::endl; 

  return;
}

int main() 
{
  myPrintHelloMake() ; 
  return 0 ; 
}



