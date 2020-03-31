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

    namespace hg
    {
    
        int hourglassSum(std::vector<std::vector<int>> arr) {
    
            static constexpr int HG_ERROR = -987654321;
            
            using matrix = std::vector<std::vector<int>> ;
            
            struct Anchor {
                int row; int col;
            };
            
            auto hg_sum = [&]( const Anchor& anchor, const matrix& m )
            {
                auto cmax = m[0].size();
                auto rmax = m.size();
                
                if( anchor.row + 3 > rmax ) return HG_ERROR;  //should not happen - would be for debug only
                if( anchor.col + 3 > cmax ) return HG_ERROR;
                
                int64_t sum = 0 ;
                for(auto i=anchor.row; i<anchor.row+3; ++i )
                    for(auto j=anchor.col; j<anchor.col+3; ++j )
                    {
                        sum += m[i][j];
                    }
                
                sum -= m[anchor.row+1][anchor.col];
                sum -= m[anchor.row+1][anchor.col+2];
                return static_cast<int>( sum );  //given that inputs are bounded - int is fine
            };
            
            const matrix& m(arr);
            auto cmax = m[0].size();
            auto rmax = m.size();
            
            std::vector<int> sums;
            for(auto i=0; i<=rmax-3; ++i )
                for(auto j=0; j<=cmax-3; ++j )
                {
                    Anchor a = { i, j };
                    int sum = hg_sum( a, m );
                    if( sum == HG_ERROR ) throw; // debugging should not happen
                    
                    sums.push_back( sum ) ;
                }
            int max = * std::max_element(sums.begin(), sums.end() );
            return max ;
        }

        namespace rotate
        {
            using std::vector ;
            vector<int> right(vector<int> a, int d)
            {
                int pivot = d % a.size();
                
                if( pivot == 0 ) return a;
                
                vector<int> result(a.end()-pivot, a.end());
                std::move(a.begin(), a.end()-pivot, std::back_inserter( result ));
                
                return result;
            }
            
            vector<int> left(vector<int> a, int d)
            {
                
                int pivot = a.size() - ( d %a.size() );
                
                return right( a, pivot );
            }
            
            
            
        };
        
        
        void solution()
        {
            {
                using matrix = std::vector<std::vector<int>>;
                
                matrix m = {
                    { 1, 1, 1, 0, 0, 0 },
                    { 0, 1, 0, 0, 0, 0 },
                    { 1, 1, 1, 0, 0, 0 },
                    { 0, 0, 2, 4, 4, 0 },
                    { 0, 0, 0, 2, 0, 0 },
                    { 0, 0, 1, 2, 4, 0 },
                };
                
                auto result = hourglassSum( m );
                assert( result == 19 );
            }
            
            {
                std::vector<int> a        = { 1,2,3,4,5};
                std::vector<int> expected = { 4,5, 1,2,3 };
                std::vector<int> result    = rotate::right( a, 2 );
                assert( result == expected );
            }
            {
                std::vector<int> a        = { 1,2,3,4,5};
                std::vector<int> expected = { 5,1,2,3,4 };
                std::vector<int> result    = rotate::left( a, 4 );
                assert( result == expected );
            }
  
            std::cout << "codility::hg success!" << std::endl;
            std::cout << "codility::rotate::left success!" << std::endl;
        };
    };
    
    
    
    
    
};

