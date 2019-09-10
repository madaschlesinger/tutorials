//
//  prime.h
//  codility
//
//  Created by Adam Schlesinger on 9/10/19.
//  Copyright © 2019 Adam Schlesinger. All rights reserved.
//
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

