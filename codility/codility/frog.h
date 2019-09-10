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
 For example, you are given integer X = 5 and array A such that:
 
 A[0] = 1
 A[1] = 3
 A[2] = 1
 A[3] = 4
 A[4] = 2
 A[5] = 3
 A[6] = 5
 A[7] = 4
 In second 6, a leaf falls into position 5. This is the earliest time when leaves appear in every position across the river.
 */


namespace codility {

    namespace frog
    {

       int cross( const std::vector<int>& leaves_by_T, int X  )  // X represents the location of the other bank
        {
            auto N = leaves_by_T.size();
            
            std::vector<bool> leaves_at_position(N,0); // current position in the river @ time T
            int icurrent = 0 ;
            
            for( auto t = 0; t<N ; ++t )
            {
                auto pos = leaves_by_T[t];
                
                leaves_at_position[ pos - 1 ] = true;
                
                while( leaves_at_position[ icurrent ] )
                {
                    if( ++icurrent >= X ) return t;
                }
            }
            
            return -1 ;
        }
        
        void solution()
        {
            {
                auto result = cross( { 1,2,3,4 }, 4 );
                assert( result == 3 );
            }
            {
                auto result = cross( { 1,2,3,4 }, 5 );
                assert( result == -1 );
            }
            {
                auto result = cross ( { 1,2,3,1 }, 4 );
                assert( result == -1  );
            }
            {
                auto result = cross( { 1,4,3,2 }, 4);
                assert( result == 3 );
            }
            
            {
                auto result = cross( { 1,3,1,4,2,3,5,4 }, 5);
                assert( result == 6 );
            }
            
            std::cout << "codility::frog success!" << std::endl;
        };
    };
    
    
    
    
    
};

