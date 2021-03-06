
#pragma once
#include <iostream>
#include <type_traits>
#include <deque>
#include <stack>
#include <set>
#include <vector>
#include <map>
#include <cmath>

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

    namespace nesting
    {
        
        int _solution(std::string s)
        {
            std::stack<char> brackets;
            static constexpr char     lbracket = '(';
            static constexpr char     rbracket = ')';

            auto is_match = [&]( char l , char r )
            {
                return l == lbracket && r == rbracket ;
            };
            
            for( auto iter = s.begin(); iter != s.end() ; ++iter )
            {
                if( *iter == lbracket )
                {
                    brackets.push( *iter );
                }
                else if( *iter == rbracket )
                {
                    if( brackets.empty() )
                        return 0;
                    
                    if( ! is_match(  brackets.top() , *iter ) )
                        return 0;
                       
                    brackets.pop();
                }
            }
            
            return brackets.empty();
        }
        
        void solution()
        {
            {
                assert( _solution( "" ) == 1 );
                assert( _solution( "()" ) == 1 );
                assert( _solution( "(())" ) == 1 );
                assert( _solution( "()()"  ) == 1 );
                assert( _solution( ""  ) ==1 );
                assert( _solution( ")"  ) == 0 );
                assert( _solution( "("  ) == 0 );
                assert( _solution( "(()"  ) == 0 );
                
                std::cout << "codility::nesting success!" << std::endl;
            }
        }
        
    }; // nesting
    

    
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

    namespace prime
    {
        std::vector<bool> initialize( int N )
        {
            std::vector<bool> v(N, true) ;
            v[0] = v[1] = false;
    
            return v;
        }
        
        std::vector<int> sieve_of_eratosthenes( int N )
        {
            auto array_bound = N + 1;
            std::vector<bool> prime = initialize(array_bound);
            
            for( auto i = 2 ; 2*i < array_bound; ++i )
            {
                for( auto j = i*i; j < array_bound; j += i )
                    prime[j]=false;
            }
            
            std::vector<int> prime_ints;
            for( auto i = 0; i < prime.size(); ++i )
                if( prime[i]) prime_ints.push_back(i);
            
            return prime_ints;
        }
        
        void solution()
        {
            {
                auto result = sieve_of_eratosthenes( 7 );
                std::vector<int> expected = { 2,3,5,7 };
                assert( result == expected );
            }
            
            {
                auto result = sieve_of_eratosthenes( 13 );
                std::vector<int> expected = { 2,3,5,7, 11, 13 };
                assert( result == expected );
            }
            
            std::cout << "codility::primes success!" << std::endl;
        };
    };
    
    namespace psuedo_prime {
        
        // number is a multiple of two prime numbers
        
        std::vector<int> numbers( int N )
        {
            auto primes = prime::sieve_of_eratosthenes( N );
            std::vector<int> result;
            
            bool done = false;
            
            for(int i = 0 ; i<primes.size() && pow(primes[i],2)<=N && ! done ; ++i )
            {
                auto sz = result.size();
                for(int j = 0 ; j<primes.size() && primes[i]*primes[j]<=N ; ++j )
                    result.push_back( primes[i] * primes[j] );
                
                if(  sz == result.size() ) done = true; // we did not add anything, so we are done
            }
            
            std::set<int> s(result.begin(), result.end() );
            result.assign(s.begin(), s.end());
            std::sort( result.begin(), result.end() );
            
            return result;
        }
        
        void solution()
        {
            {
                constexpr int N = 7;
                
                std::vector<int> expected = { 4,6 }; // 2*2, 2*3
                std::vector<int> result   = psuedo_prime::numbers( N );
                
                assert( result == expected );
            }
            
            {
                constexpr int N = 26;
                
                std::vector<int> expected = { 4, 6, 9, 10, 14, 15, 21, 22, 25, 26 }; // 2*2, 2*3
                std::vector<int> result   = psuedo_prime::numbers( N );
                
                assert( result == expected );
            }
            std::cout << "codility::psuedo_prime success!" << std::endl;
        };
        
    };


    
    
};

