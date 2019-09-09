#pragma once
#include <iostream>
#include <vector>

 using std::vector;

namespace detail
{
    
    template <typename T>
    struct Allocator
    {
        static T *allocate(size_t capacity )
        {
            return new T[capacity];
        }
        
        static void free( T* buf )
        {
            delete [] buf;
        }
    };
    
    template <typename T, typename ALLOCATOR=detail::Allocator<T>>
	struct Vector
    {
        static constexpr size_t CAPACITY = 512;
        
        Vector( size_t capacity = Vector::CAPACITY ) : size_(0), capacity_(capacity)
        {
            buffer_   = ALLOCATOR::allocate(capacity);
            memset(buffer_,0,capacity*sizeof(T) );
        }

        Vector( const std::vector<T>& v )
        : Vector( v.capacity() )
        {
            // now copy elements
            //for( auto i = 0 ; i != v.size() ; ++i )
            //{
            //    buffer_[i] = v[i];
            //    auto cc = v[i];
            // }
            
            std::copy(v.begin(), v.end(), begin() );

            size_     = v.size();
        }
        
        Vector( const std::initializer_list<T> i)
        : Vector( std::vector<T>(i) )
        {}
        
        Vector( const Vector<T>& v )
        : Vector( v.capacity() )
        {
            auto c = v.capacity_;
            // now copy elements
            memcpy( buffer_, v.buffer_, v.size_ *sizeof(T) );
            
            size_ = v.size_;
        }
        
        
        Vector( Vector<T>&& v ) : buffer_( v.buffer_ ), size_( v.size_), capacity_( v.capacity_ )
        {
            v.buffer_ = nullptr;
            v.capacity_ = v.size_ = 0 ;
        }

        ~Vector()
        {
            ALLOCATOR::free( buffer_ );
            buffer_=nullptr;
        }
        
        const T& operator[](size_t index ) const
        {
            if( index < size_ ) return buffer_[index];
            else throw std::exception( std::out_of_range( "index gt than size ") );
            
        }
        
        T& operator[](size_t index )
        {
            if( index < size_ ) return buffer_[index];
            else throw std::exception( std::out_of_range( "index gt than size " ));
        }
        
        
        void push_back( const T& element )
        {
            if( size_ == capacity_ ) resize();
            
            buffer_[size_++] = element;
        }
        
        void resize()
        {
            T *tbuf = buffer_;
            buffer_ = ALLOCATOR::allocate( capacity_ * 2 );
            
            memcpy( buffer_, tbuf, size_*sizeof(T) );
            
            capacity_ *= 2;
            
            ALLOCATOR::free( tbuf );
        }
        
        T*       begin()       { return buffer_ ;}
        const T* begin() const { return buffer_ ;}
        const T* end()   const { return buffer_ + size_ ;}
        
        size_t  size()     const { return size_ ;}
        size_t  capacity() const { return capacity_ ;}

        T*             buffer_     = nullptr ;
        size_t         size_       = 0;
        size_t         capacity_   = Vector::CAPACITY;
    }
    ;

}
void _vector()
{

	
 
	using detail::Vector;
	
    Vector<int> v(2);
    
    Vector<int> x = { 1, 2, 3 };
    
    assert( x[0]    == 1 );
    assert( x[1]    == 2 );
    assert( x[2]    == 3 );

    Vector<int> x2( x );
    
    assert( x[0]    == 1 );
    assert( x[1]    == 2 );
    assert( x[2]    == 3 );
    assert( x2[0]   == 1 );
    assert( x2[1]   == 2 );
    assert( x2[2]   == 3 );
    
    
    assert( x.size() == 3 );
    assert( x2[1]    == 2 );

    x2.push_back( 4 );  // test resize

    assert( x[0]  == 1 );
    assert( x[1]  == 2 );
    assert( x[2]  == 3 );
    assert( x2[0] == 1 );
    assert( x2[1] == 2 );
    assert( x2[2] == 3 );
    assert( x2[3] == 4 );
    
    x2[3] = 5;
    assert( x2[3] == 5 );
    std::cerr << "vector(success)." << std::endl ;
}

