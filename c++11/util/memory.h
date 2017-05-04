#ifndef __memory_h__
#define __memory_h__


#include <util/backing_store.h>
#include <typeinfo>


namespace util { namespace memory { 

    using util::backing_store::store ; 
    using util::backing_store::header;
    using util::backing_store::fs_traits;
    
    // T is the object we will be allocating memory for
    template<typename T, typename S = fs_traits >
    class allocator
    {
    public:
      typedef T        value_type      ; 
      typedef T*       pointer         ;
      typedef T&       reference       ;
      typedef const T* const_pointer   ;
      typedef const T& const_reference ;
      
      typedef size_t    size_type ; 
      
    private:
      
      S           traits_ ; 
      store<S>    store_  ; 
      
      header *    hdr_ptr_;
      pointer     memory_ ;
      pointer     rd_ptr_;
      pointer     wr_ptr_;
      uint32_t    n_;         // n actually allocated objects - i.e. used
      uint32_t    N_;         // total objects for which raw space is 'reserved' - max_size
      
    public:
      
      
      // TODO - this should take traits class as optional - not individial parameter
      // TODO - shm impl -then perhaps fns move to store and out of traits
      // TODO - reserve???? something needs to have the wr_ptr functionality -
      //      - and now when crossing boundary
      allocator(  const S& t = S() ) 
        : store_( t )
      {
#if 0 
        std::cerr <<"allocator hash["<< typeid( value_type ).hash_code() << "]" << std::endl;
        std::cerr <<"allocator type["<< typeid( value_type ).name()      << "]" << std::endl;

        http://en.cppreference.com/w/cpp/types/type_info/hash_code
        https://github.com/johannesthoma/mmap_allocator
        
#endif

        
        uint32_t N = store_.traits.sz / sizeof( value_type ) ; 
        initialize(N) ; 
      }
      
      allocator(  uint32_t N ) 
        : store_( S() )
      {
        initialize(N) ; 
      }
      
      
      virtual ~allocator()
      {
      }
      
      const size_t capacity() const { return store_.capacity() - sizeof( header ); }  // space - bytes
      const size_t max_size() const { return N_ ; }                                   // space - n objects
      const size_t size()     const { return n_ ; }                                   // space - n objects
      
    private:
            
            void * initialize(uint32_t N)
                {
                    store_.traits.sz = N * sizeof( value_type ) ; 
                    memory_          = static_cast<pointer>( store_.open_and_mmap() )  ;
                    hdr_ptr_         = store_.hdr_ptr()         ; 
                    
                    // TODO - 
                    // layout top of memory to be the local header information
                    // which means - the 'stats' struct must be mmap itself,
                    // and the non ptr elements need to be in a local struct
                    // w/mmap'd memory
                    
                    if( memory_ == nullptr ) 
                    {
                        return nullptr ; 
                    }
              
                    rd_ptr_ = static_cast<pointer>( memory_ ) ;
                    wr_ptr_ = static_cast<pointer>( memory_ ) ;
                    n_      = 0 ;
                    N_      = capacity()/ sizeof( value_type ) ; 

                    if( store_.traits.truncate == false )
                    {
                        n_       = N_ ; 
                        wr_ptr_ += N_ ; 
                    }
                    
                    return memory_ ; 
                }
        
          public:
            pointer allocate( size_type n = 1 )
                {
                    // TODO - specialise this.... 'allocate' is not purely correct in
                    //                            terms of a ro trait.... - plus truncate
                    //                            is clumsy - borderline incorrect
                    //                            bind for read only usage might be more applicable
                    if( store_.traits.truncate == false ) return rd_ptr_  ;
                    
                    if( ! has_spare_capacity( n ) ) return nullptr ; // std::allocator would throw bad_alloc... 
              
                    pointer mem   = wr_ptr_ ;
                    wr_ptr_      += n  ; 
                    n_           += n  ;
                    hdr_ptr_ -> n = n_ ;  

                    return mem ; 
                }

            bool has_spare_capacity( uint32_t n = 1 ) // # in 'N' object terms - not in sizeof(T) terms
                {
                    // not really a great fn.... if this is readonly - this is always false...
                    // might be cleaner to do this all w/pointers - & have an end_ptr and compare wr_ptr +n <end_ptr 
                    return ( ( n_ + n ) <= N_  ) ; 
                }

          public:
          
        };
        
    }; // ns allocator
    
}; // ns util


#endif


