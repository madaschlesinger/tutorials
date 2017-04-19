#include <stdio.h>
#include <iostream>
#include <vector>
#include <typeinfo>
#include <functional>

using std::function ; 

void fibonacci()
{
  std::cerr << "_fibonacci() : " ; 

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

  std::cerr << "_fibonacci() : " ; for(auto i=1; i < 10; ++i ) std::cerr << fibonaci(i) << " ";  
  std::cerr << std::endl;
}

