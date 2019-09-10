#pragma once
#include <iostream>
#include <type_traits>
#include <deque>
#include <stack>
#include <set>
#include <vector>
#include <map>
#include <cmath>

/*
 A permutation is a sequence containing each element from 1 to N once, and only once.
 
 For example, array A such that:
 
 A[0] = 4
 A[1] = 1
 A[2] = 3
 A[3] = 2
 is a permutation, but array A such that:
 
 A[0] = 4
 A[1] = 1
 A[2] = 3
 is not a permutation, because value 2 is missing.
 */


namespace codility {

    namespace permutations
    {

        
       int is_permutation( const std::vector<int>& v  )
        {
            auto N = v.size();
            
            std::vector<int> counts(N,0);
            
            for( auto i = 0; i<N ; ++i )
            {
                if( v[i] >  N )        return 0;
                if( counts[v[i]-1]++ ) return 0;
            }
            
            return 1;
        }
        
        void solution()
        {
            {
                auto result = is_permutation( { 1,2,3,4 });
                assert( result == true );
            }
            {
                auto result = is_permutation( { 1,2,3,5 });
                assert( result == false );
            }
            {
                auto result = is_permutation( { 1,2,2,4 });
                assert( result == false );
            }

  
            
            std::cout << "codility::permutations success!" << std::endl;
        };
    };
    
    
    
    
    
};

