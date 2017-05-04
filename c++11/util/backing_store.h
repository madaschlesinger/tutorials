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
#include <functional>


namespace util { namespace backing_store {

        using std::cout;
        using std::cerr;
        using std::endl;
        using std::string;

        enum class backing_store_type : std::int8_t
        {
            SHARED_MEMORY = 1,
                FILE_SYSTEM   = 2,
                MEMORY        = 3,
                ANONYMOUS     = 4
                };
        

        // FIXME - this is an improper use of terminology
        //         traits classes should have static members
        //         and static functions typedefs - not instance
        //         members

        //         as a follow up - the 'default_' should be remove
        //         along w/the instance variables and ctrs
        
        // Traits Class - for initial size, page increase
        // backing store mode possibly
        // alternative - should there be config in here....
        // READ vs. WRITE - could be in traits  - which could 
        // control open modes.
        // shm fstore could all be traits too
        struct traits
        {
          // int         fd;
            string      fname;
            size_t      sz          ; // TODO - need to think about block size -growth, pages/hugepages
            int         oflags      ; // open flags
            int         pflags      ; // mmap protection flags
            int         mflags      ; // mmap flags
            mode_t      permissions ; // file permission
        
            static const bool truncate    = true    ; 
        
            std::function<int(const char *,int,mode_t)> open_func = ::open; 
    
            static int    default_flags_r()        { return  O_RDONLY ; }
            static int    default_flags_rw()       { return  O_RDWR | O_CREAT | O_TRUNC; }
            static int    default_flags()          { return  default_flags_rw() ; } 
            static size_t default_capacity()       { return  1024 * 1024 ; }
            static string default_fname()          { return  "/tmp/backing_store.$(pid).bin" ; }
            static mode_t default_permissions()    { return  (mode_t) ( S_IRUSR | S_IWUSR | S_IRGRP ) ; }

            traits(  string n     = traits::default_fname() ,
                     size_t s     = traits::default_capacity() ,
                     int    f     = traits::default_flags() ,
                     mode_t perms = traits::default_permissions() )
                : fname( n ), sz( s ), oflags( f )
                {
                    permissions = (mode_t) perms ;
                    pflags      = PROT_READ | PROT_WRITE ;
                    mflags      = MAP_SHARED ;

                    std::string::size_type pos = fname.find( "$(pid)" ) ;

                    if( pos != std::string::npos ) 
                      {
                        pid_t pid = getpid();
                        std::string tag = std::to_string(  (int) pid ) ;
                        fname.replace( pos, 6, tag); 
                      }

                    // TODO $(unique) - need to be at the open/create however

                }
          
            virtual ~traits()
                {
                }

        };

        typedef traits  fs_traits ;
        
        struct fs_traits_ro : public fs_traits
        {
          // TODO - size is an issue here
          //        should check the filesize really - not the compiled
          //        values
          static const bool truncate = false ;
          
          static int    default_flags()        { return  O_RDONLY ; }
          
            fs_traits_ro(  string n     = traits::default_fname() ,
                           size_t s     = 0,
                           int    f     = fs_traits_ro::default_flags() ,
                           mode_t perms = traits::default_permissions() )
                : fs_traits(n,s,f,perms )
                {
                    assert( this -> oflags  == O_RDONLY ) ;

                    // this -> truncate = false ;
                    this -> pflags   = PROT_READ ; 
                }
        };

        struct shm_traits : public traits
        {
          static size_t default_capacity()     { return  16 * 1024 ; }
          static int    default_flags()        { return  O_CREAT | O_RDWR; } // { return  O_CREAT | O_EXCL | O_RDWR; }
          static string default_fname()        { return  "backing_store.bin" ; }
          
            shm_traits(  string n     = shm_traits::default_fname() ,
                         size_t s     = shm_traits::default_capacity(),
                         int    f     = shm_traits::default_flags() ,
                         mode_t perms = shm_traits::default_permissions() )
                : traits(n,s,f,perms )
                {
                    assert( this -> oflags       == O_CREAT | O_RDWR  ) ;
                    assert( this -> permissions  == S_IRUSR | S_IWUSR | S_IRGRP ) ;

                    this -> pflags    = PROT_WRITE | PROT_READ ;
                    this -> mflags    = MAP_SHARED | MAP_LOCKED | MAP_POPULATE ; 
                    // this -> truncate  = true ;
                    this -> open_func = ::shm_open; 
                }
        };

        // need to determine growth - compile_time - then is initial size etc. 
        struct shm_traits_ro : public shm_traits
        {
            static int    default_flags()        { return  O_RDWR; } // { return  O_CREAT | O_EXCL | O_RDWR; }

            shm_traits_ro(  string n     = shm_traits::default_fname() ,
                            size_t s     = 0,
                            int    f     = shm_traits_ro::default_flags() ,
                            mode_t perms = shm_traits::default_permissions() )
                : shm_traits(n,s,f,perms)
                {
                    this -> pflags    = PROT_READ ;
                }

        };
    
        struct header
        {
            uint32_t n         = 0 ; // number of objects in the store
            char     reserved[ 128 - sizeof( uint32_t ) ] ; 
        };

        // design decision - this is raw type - i.e. not related to the type of object being
        // stored in it.... - may need to rethink in the future.
        template<typename T = traits >
        class store
        {
          public:
            T          traits; 
          private:
            
            int         fd_; 
            header*     header_     = nullptr ;             // mapped memory - starts at mmap 
            void *      memory_     = nullptr ;             // mapped memory + starts at mmap + offset header
            size_t      capacity_   = 0 ;                   // stl - consistent w/capcity usage in stl
            size_t      capacity_i_ = 0 ;                   // initial  capacity value - used only during creation
            
            // error handling
            string  last_error_ ; 
            bool    success_    ;          // tracks a 'good' state
            
            
          public:
            
            // TODO - shm impl -then perhaps fns move to store and out of traits
            // TODO - reserve???? something needs to have the wr_ptr functionality -
            //      - and now when crossing boundary
            store(  const T& t = T() )
                : traits( T() ) // t.fname, t.sz, t.oflags, t.permissions )
                {
                    capacity_i_   = t.sz + sizeof( header ) ;
                    capacity_     = 0 ; 
                }

            store(  uint32_t c_i )
                : traits( T() )
                {
                    capacity_i_  = c_i + sizeof( header ) ;
                    capacity_    = 0 ;                     
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
            
            const string& last_error() const { return last_error() ; }
            
            bool operator!() const 
                {
                    return ! success_ ; 
                }
          private:
            
            int open()
                {
                    
                    /* Open a file for writing.
                     *  - Creating the file if it doesn't exist.
                     *  - Truncating it to 0 size if it already exists. (not really needed)
                     *
                     * Note: "O_WRONLY" mode is not sufficient when mmaping.
                     */
                    
                    fd_ = traits.open_func( traits.fname.c_str(), traits.oflags, traits.permissions );
                    
                    if( fd_ == -1 )
                    {
                        success_    = false; 
                        last_error_ = "Error opening file." ; 
                        return fd_ ; 
                    }
                    
                    struct stat stats ;
                    
                    if( fstat(fd_, &stats ) >=0 && stats.st_size > capacity_ )
                    {
                        capacity_ = stats.st_size ; 
                    }
                    
                    
                    if( capacity_i_ && traits.truncate ) // awkward - this is more a readonly vs. create/rw access
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
                    memory_ =  ::mmap(NULL, capacity_, traits.pflags, traits.mflags, fd_, 0);
                    header_ = static_cast<header *>( memory_ ) ; 

                    if( memory_== MAP_FAILED)
                    {
                        this -> close();
                        success_    = false; 
                        last_error_ = "Error: mmaping the file: " + string( ::strerror( errno ) ) ;
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
            void *allocate_next_block() - not even correct naming - allocate is used from a caller perpective
                here we are resizoing - or extending capcity() 
                {
                    // TODO = need to keep track of this - possibly chnage sz -> block_size
                    size_t sz  = capacity_ * 2;
                    int result = ::ftruncate( fd_, sz ) ;
        
                    if( result != 0 )
                    {
                        success_    = false; 
                        last_error_ = "Error: ftruncate has failed with error code : " + errno ;
                        return nullptr ; 
                    }

                    capacity_ = sz ; 
                    // TODO - TEMP - interim ci ONLY - need to remmap
                    return  nullptr;
                }
#endif     
        };
    
    }; // ns allocator
    
}; // ns util


#endif


