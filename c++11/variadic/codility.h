
#pragma once
#include <iostream>
#include <type_traits>
#include <deque>
#include <stack>
#include <set>
#include <map>

namespace codility {
    
    namespace brackets
    {
    
        int _solution(std::string s)
        {
            std::stack<char> brackets;
            static const std::set<char>        lbrackets = { '{', '(', '[' };
            static const std::set<char>        rbrackets = { '}', ')', ']' };
            static const std::map<char,char>   mbrackets = { { '}','{' }, {']','[' } , { ')','(' } };
            
            auto is_match = [&]( char closing_bracket , char opening_bracket )
            {
                auto iter = mbrackets.find( closing_bracket) ;
                if( iter == mbrackets.end() ) return false;
        
                return iter->second == opening_bracket;
            };
            
            for( auto iter = s.begin(); iter != s.end() ; ++iter )
            {
                if( lbrackets.find( *iter ) != lbrackets.end() )
                {
                    brackets.push( *iter );
                    continue;
                }
                if( rbrackets.find( *iter ) != rbrackets.end() )
                {
                    if( brackets.empty() )
                        return 0;
                    
                    if( ! is_match( *iter, brackets.top() ) )
                        return 0;
                    brackets.pop();
                }
            }
            
            return brackets.empty();
        }
        
        void solution()
        {
            {
                assert( _solution( "{[()()]}" ) == 1 );
                assert( _solution( "" ) == 1 );
                assert( _solution( "{}" ) == 1 );
                assert( _solution( "{[]}"  ) == 1 );
                assert( _solution( "{[abc]q}"  ) ==1 );
                assert( _solution( "{[]"  ) == 0 );
            }
        }
        
    } // brackets
        
    namespace fish { // need more tests cases - this only scored a 50...
        
        int _solution(vector<int> &A, vector<int> &B)
        {
            struct fish
            {
                fish(int s, int d ) :size(s),direction(d) {}
                int size;
                int direction;
                
                bool operator<( const fish& rhs ) const
                {
                    return this->size < rhs.size ;
                }
                
            };
            
            auto swimming_in_same_direction = [](const fish& curr_fish, const fish& prev_fish )
            {
                return curr_fish.direction == prev_fish.direction;
            };
            
            auto is_swimming_away_from_each_other = [](const fish& curr_fish, const fish& prev_fish )
            {
                return curr_fish.direction == 1 && prev_fish.direction == 0 ;
            };
            auto prev_eats_curr = []( const fish& curr_fish, const fish& prev_fish )
            {
                return curr_fish.size < prev_fish.size ;
            };
            
            std::stack<fish> fishes;
            
            if( A.empty() || B.empty() ) return 0;
            
            fishes.push( fish(A[0], B[0] ));
            
            for(int i = 1; i < A.size(); ++i )
            {
                fish  curr_fish(A[i], B[i]);
                fish& prev_fish = fishes.top() ;
                
                if( swimming_in_same_direction( curr_fish, prev_fish ) )
                {
                    // we are past it and swiming in the same direction
                    fishes.push( curr_fish );
                    continue;
                }
                if( is_swimming_away_from_each_other( curr_fish, prev_fish))
                {// we are past it and swiming away
                    fishes.push( curr_fish );
                    continue;
                }
                else if( prev_eats_curr( curr_fish, prev_fish ) )
                    continue ;  // current fish is eaten
                else
                {   // current fish is bigger - it eats as many as you can, then move put biggest fish on this stack
                    while ( ! fishes.empty() )
                    {
                        fish& last_fish = fishes.top() ;
                        if( last_fish < curr_fish && last_fish.direction != curr_fish.direction )
                        {
                            fishes.pop();
                            
                            if( ! fishes.empty() )
                                continue;
                        }
                        fishes.push( curr_fish );
                        break;
                    }
                }
                
            }
            
            return static_cast<int>( fishes.size() );
        }
        
        void solution()
        {
            {
                std::vector<int> A = { 4, 3, 2, 1, 5 },     B= { 0,1,0,0,0};
                assert( _solution(A,B) == 2 );
            }
            
            
            {
                std::vector<int> A = { 4, 3, 2, 1, 5 },     B= { 0,0,0,0,0};
                assert( _solution(A,B) == 5 );
            }
            
            
            {
                std::vector<int> A = { 4, 3, 2, 1, 5 },     B= { 1,1,1,1,0};
                assert( _solution(A,B) == 1 );
            }
        }
    
    } // fish

    namespace oldmission { // need more tests cases - this only scored a 50...
        
        int _solution(vector<int> &A, vector<int> &B)
        {
            return 0;
        }
        
        void solution()
        {
        }
        
    } // old mission


    
    
};

