#ifndef __memory_h__
#define __memory_h__


#include <util/backing_store.h>
#include <typeinfo>


namespace util { namespace memory { 

    using util::backing_store::store ; 
    using util::backing_store::header;
    using util::backing_store::fs_traits;


    template<typename traits = fs_traits>
    struct allocator_traits
    {
      // missing some template typedef - causing declaration problems...
      typedef traits store;

      static std::string filename() { return std::string( "/tmp/allocator.$(pid).bin" ); } 
      

    };
    
    // T is the object we will be allocating memory for
    template<typename T, typename traits = fs_traits >
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
      
      store<traits> store_ ; 
      
      header *      hdr_ptr_;
      pointer       memory_ ;
      pointer       rd_ptr_;
      pointer       wr_ptr_;
      uint32_t      n_;         // n actually allocated objects - i.e. used
      uint32_t      N_;         // total objects for which raw space is 'reserved' - max_size
      
    public:
      
      allocator( std::string store_name = std::string( "/tmp/allocator.$(pid).bin" ) )
        : store_( store_name ) // this is not kosher....
      {
        uint32_t N = traits::initial_capacity / sizeof( value_type ) ; 
        initialize(N) ; 
      }
      
      allocator(  uint32_t N, std::string store_name = std::string( "/tmp/allocator.$(pid).bin" ) ) 
        : store_( store_name )
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
        //
        memory_          = static_cast<pointer>( store_.open_and_mmap() )  ;
        hdr_ptr_         = store_.hdr_ptr()         ; 
                  
        if( memory_ == nullptr ) 
          {
            return nullptr ; 
          }
              
        rd_ptr_ = static_cast<pointer>( memory_ ) ;
        wr_ptr_ = static_cast<pointer>( memory_ ) ;
        n_      = 0 ;
        N_      = capacity()/ sizeof( value_type ) ; 

        if( traits::truncate == false )
          {
            n_       = hdr_ptr_ -> n ; 
            wr_ptr_ += N_ ;

            size_t nn = hdr_ptr_ -> n ; 
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
        if( traits::truncate == false ) return rd_ptr_  ;
                    
        if( ! has_spare_capacity( n ) ) return nullptr ; // std::allocator would throw bad_alloc... 
              
        pointer mem   = wr_ptr_ ;
        wr_ptr_      += n  ; 
        n_           += n  ;
        hdr_ptr_ -> n = n_ ; // should really be a user area w/in the header....  

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


