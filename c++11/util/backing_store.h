#ifndef __backing_store_h__
#define __backing_store_h__


#include <iostream>
#include <cassert>
#include <string>

// Gestion des erreurs
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <cstdint>
#include <climits>
#include <typeindex>
#include <functional>


namespace util { namespace backing_store {

    enum class backing_store_type : std::int8_t
    {
      SHARED_MEMORY = 1,
        FILE_SYSTEM   = 2,
        MEMORY        = 3,
        ANONYMOUS     = 4
        };
        
    template<backing_store_type BsType, bool RW = true >
    struct traits
    {
      //size_t      sz          ; // TODO - need to think about block size -growth, pages/hugepages
         
      static const bool   truncate         = RW   ;
      static const int    file_open_flags  = O_RDWR | O_CREAT | O_TRUNC;
      static const mode_t file_permissions = (mode_t) ( S_IRUSR | S_IWUSR | S_IRGRP ) ;
      static const int    mmap_flags       = MAP_SHARED ;
      static const int    mmap_protection  = PROT_READ | PROT_WRITE ;

      static const bool   is_readonly      = ! RW ;

      static const size_t initial_capacity = 1024 * 1024 ;

      static std::string filename() { return std::string( "/tmp/bs.$(pid).bin" ); } 
          
      // std::function<int(const char *,int,mode_t)> open_func = ::open; 
    
      static std::string file_name( std::string filename , std::string unique_id = "" ) // qualified filename
      {

        std::string::size_type pos = filename.find( "$(pid)" ) ;

        if( pos != std::string::npos ) 
          {
            pid_t pid = getpid();
            std::string tag = std::to_string(  (int) pid ) ;

            if( unique_id.size() )  tag += "." + unique_id  ; 
            
            filename.replace( pos, 6, tag); 
          }

        // $(uniq)
            
        return filename; 
      }


      static int open( const char* filename, int flags, mode_t permissions )
      {
        return ::open( filename, flags, permissions );
      }

    };

    typedef traits<backing_store_type::FILE_SYSTEM, true>  fs_traits ;
        
    struct fs_traits_ro : public traits<backing_store_type::FILE_SYSTEM, false> 
    {
      static const int  file_open_flags  = O_RDONLY ; 
      static const int  mmap_protection  = PROT_READ;
    };


    struct shm_traits : public traits<backing_store_type::SHARED_MEMORY,true>
    {

      static const int         file_open_flags  = O_CREAT | O_RDWR ;
      static const int         mmap_flags       = MAP_SHARED | MAP_LOCKED | MAP_POPULATE  ; 
      static const size_t      initial_capacity = 16 * 1024 ;

      static std::string filename() { return std::string( "bs.$(pid).bin" ); }

      static int open( const char* filename, int flags, mode_t permissions )
      {
        return ::shm_open( filename, flags, permissions );
      }
          
          
    };
        
    // need to determine growth - compile_time - then is initial size etc. 
    struct shm_traits_ro : public shm_traits
    {
      static const int    file_open_flags  = O_RDONLY ; 
      static const int    mmap_protection  = PROT_READ;
      static const bool   truncate         = false    ;
      static const bool   is_readonly      = true     ;
    };
    
    struct header
    {
      uint32_t n         = 0 ; // number of objects in the store
      char     reserved[ 128 - sizeof( uint32_t ) ] ; 
    };

    // design decision - this is raw type - i.e. not related to the type of object being
    // stored in it.... - may need to rethink in the future.
    template<typename traits = fs_traits >
    class store
    {
    private:
          
      int         fd_; 
      header*     header_     = nullptr ;             // mapped memory - starts at mmap 
      void *      memory_     = nullptr ;             // mapped memory + starts at mmap + offset header
      size_t      capacity_   = 0 ;                   // stl - consistent w/capcity usage in stl
      size_t      capacity_i_ = 0 ;                   // initial  capacity value - used only during creation
      std::string filename_       ; 
      
      // error handling
      std::string  last_error_ ; 
      bool         success_    ;          // tracks a 'good' state
          
          
    public:
          
      // TODO - reserve???? something needs to have the wr_ptr functionality -
      //      - and now when crossing boundary
      // TODO - filename processing - has to move into store
      store( std::string filename = traits::filename() )
      {
        capacity_i_   = traits::initial_capacity + sizeof( header ) ;
        capacity_     = 0 ;
        filename_     = filename ; 
      }
          
      store(  uint32_t c_i,  std::string filename = traits::filename() )
      {
        capacity_i_  = c_i + sizeof( header ) ;
        capacity_    = 0 ;
        filename_    = filename ; 
      }
          
          
      virtual ~store()
      {
        try
          {
            if( memory_ != nullptr ) this -> munmap()  ; 
            if( fd_     !=-1       ) this -> close()   ;  
          }
        catch( ... ) 
          {
            // TODO - error processing?? Logger - not much else to do 
          }
      }
          
      //
          
      header *hdr_ptr() { return header_ ; } 
          
      // like stl - capacity is the total allocated - i.e. not the total used
      size_t capacity() const { return capacity_ ; } 
          
      // static??? - all of it could be static/templated..... 
          
      const std::string& last_error() const { return last_error() ; }
          
      bool operator!() const 
      {
        return ! success_ ; 
      }
    private:
          
      int open()
      {
        // Note: "O_WRONLY" mode is not sufficient when mmaping.
        // TODO - need to decide on filename &| hashid b/c hash code for allocator needs to be here
        fd_ = traits::open( traits::file_name( filename_ ).c_str(),
                            traits::file_open_flags,
                            traits::file_permissions );
            
        if( fd_ == -1 )
          {
            success_    = false; 
            last_error_ = "Error opening file: " + std::string( ::strerror( errno ) ) ;
                
            return fd_ ; 
          }
            
        struct stat stats ;
            
        if( fstat(fd_, &stats ) >=0 && stats.st_size > capacity_ )
          {
            capacity_ = stats.st_size ; 
          }
            
            
        if( capacity_i_ && traits::truncate ) 
          {
            if( this -> ftruncate() != 0 ) return -1 ; 
          }
            
        success_ = true; 
        return fd_; // ??? keeping the open symantic - but breaks encapsulation
      }
          
      int close() 
      {
        int result = ::close( fd_ );
        fd_        = -1 ;
        return result ;
      }     
          
      int ftruncate()
      {
        // if we are here.... we are in wr mode - the file, may - or may not have
        // existed....
            
        size_t lsize  = std::max(    capacity_, capacity_i_ );
        int    result = ::ftruncate( fd_,       lsize ) ;
            
        if( result != 0 )
          {
            success_    = false; 
            last_error_ = "Error: ftruncate has failed with error code : " + errno ;
            return result ; 
          }
            
        capacity_ = lsize ; 
            
        return result ;
      }
          
          
      void *mmap()
      {        
        memory_ =  ::mmap(NULL, capacity_, traits::mmap_protection, traits::mmap_flags, fd_, 0);
        header_ = static_cast<header *>( memory_ ) ; 
            
        if( memory_== MAP_FAILED)
          {
            this -> close();
            success_    = false; 
            last_error_ = "Error: mmaping the file: " + std::string( ::strerror( errno ) ) ;
            return nullptr ; 
          }
            
        memory_ = (void *) ( header_ + 1 ) ; 
            
        return memory_;
      }
          
      int munmap() 
      {
        int result = ::munmap( header_, capacity_ ) ; 
            
        if( result != 0) 
          { 
            success_    = false; 
            last_error_ = "Error: munmap has failed with error code : " + errno ;
            
            return result ; 
          }
                    
        header_ = nullptr ; 
        memory_ = nullptr ; 

        return result;
      }
    
    public:
    
      void *open_and_mmap()
      {
        open() ; 
    
        if( ! success_ ) return nullptr; // MAP_FAILED

        return mmap() ; 
      }

#if 0             
      /* void *allocate_next_block() - not even correct naming - allocate is used from a caller perpective */
      /*     here we are resizoing - or extending capcity()  */
      /*     { */
      /*         // TODO = need to keep track of this - possibly chnage sz -> block_size */
      /*         size_t sz  = capacity_ * 2; */
      /*         int result = ::ftruncate( fd_, sz ) ; */
        
      /*         if( result != 0 ) */
      /*         { */
      /*             success_    = false;  */
      /*             last_error_ = "Error: ftruncate has failed with error code : " + errno ; */
      /*             return nullptr ;  */
      /*         } */

      /*         FIXME capacity_ = sz ;  */
      /*         // TODO - TEMP - interim ci ONLY - need to remmap */
      /*         return  nullptr; */
    }
#endif     
  };
    
}; // ns allocator
    
}; // ns util


/*
 * working on filename unquieness.....  typeid stuff is not guaranteed accross invocations - 
 *       std::string unique_id =  std::to_string( typeid(double).name() );
 *
 *      if( typeid( T ) !=  typeid( decltype( nullptr ) ) )
 *        {
 *          unique_id = std::to_string( typeid(T).name() );
 *        }
 *
 */
#endif


