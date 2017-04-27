#ifndef __backing_store_h__
#define __backing_store_h__


#include <iostream>
#include <cassert>
#include <string>

// Gestion des erreurs
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
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

    enum class backing_store_type : std::int8_t { SHARED_MEMORY = 1, FILE_SYSTEM =2, MEMORY = 3, ANONYMOUS =4  };
    
    
    // Traits Class - for initial size, page increase
    // backing store mode possibly
    // alternative - should there be config in here....
    // READ vs. WRITE - could be in traits  - which could 
    // control open modes.
    // shm fstore could all be traits too
    struct traits
    {
        int         fd;
        string      fname;
        size_t      sz;         // TODO - need to think about block size -growth, pages/hugepages
        int         oflags      ; // open flags
        int         pflags      ; // mmap protection flags
        int         mflags      ; // mmap flags
        mode_t      permissions ;
        void *      memory      = nullptr ; // mapped memory
        bool        truncate    = true    ; 

        // error handling
        string  last_error ; 
        bool    success    ;          // tracks a 'good' state

        std::function<int(const char *,int,mode_t)> open_func = ::open; 
    
        static int    default_flags_r()        { return  O_RDONLY ; }
        static int    default_flags_rw()       { return  O_RDWR | O_CREAT | O_TRUNC; }
        static int    default_flags()          { return  default_flags_rw() ; } 
        static size_t default_size()           { return  4 * 1024 * 1024 ; }
        static string default_fname()          { return  "/tmp/backing_store.bin" ; }
        static mode_t default_permissions()    { return  (mode_t) ( S_IRUSR | S_IWUSR | S_IRGRP ) ; }

      traits(  string n     = traits::default_fname() ,
               size_t s     = traits::default_size() ,
               int    f     = traits::default_flags() ,
               mode_t perms = traits::default_permissions() )
      : fname( n ), sz( s ), oflags( f ), success(  true ) 
            {
                permissions = (mode_t) perms ;
                pflags      = PROT_READ | PROT_WRITE ;
                mflags      = MAP_SHARED ; 
            }

        virtual ~traits()
            {
                try
                {
                    if( memory != nullptr ) this -> munmap()  ; 
                    if( fd     !=-1       ) this -> close()   ;  
                }
                catch( ... ) 
                {
                    // TODO - error processing?? Logger - not much else to do 
                }
            }
        
        void * mapped_memory() { return memory ; }

        int open()
            {
            
                /* Open a file for writing.
                 *  - Creating the file if it doesn't exist.
                 *  - Truncating it to 0 size if it already exists. (not really needed)
                 *
                 * Note: "O_WRONLY" mode is not sufficient when mmaping.
                 */
            
                fd = open_func( fname.c_str(), oflags, permissions );
            
                if( fd == -1 )
                {
                    success    = false; 
                    last_error = "Error opening file." ; 
                    return fd ; 
                }

                struct stat stats ;

                if( fstat(fd, &stats ) >=0 && stats.st_size > sz )
                {
                    sz = stats.st_size ; 
                }

                
                if( sz && truncate ) // might not be good for shm.... not quite 'trait'- ish
                {
                
                    if( this -> ftruncate() != 0 ) return -1 ; 
                }
            
                success = true; 
                return fd; // ??? keeping the open symantic - but breaks encapsulation
            }
    
        int ftruncate()
            {
                int result = ::ftruncate( fd, sz ) ;
            
                if( result != 0 )
                {
                    success    = false; 
                    last_error = "Error: ftruncate has failed with error code : " + errno ;
                    return result ; 
                }
            
                return result ;
            }


        void *allocate_next_block()
            {
                // TODO = need to keep track of this - possibly chnage sz -> block_size
                sz *= 2;
                int result = ::ftruncate( fd, sz ) ;
            
                if( result != 0 )
                {
                    success    = false; 
                    last_error = "Error: ftruncate has failed with error code : " + errno ;
                    return nullptr ; 
                }

                // TODO - TEMP - interim ci ONLY - need to remmap
                return  nullptr;
            }

        void *mmap()
            {
            
                memory =  ::mmap(NULL, sz, pflags, mflags, fd, 0);
            
                if( memory== MAP_FAILED)
                {
                    this -> close();
                    success    = false; 
                    last_error = "Error: mmaping the file: " + errno ;
                    return nullptr ; 
                }

                return memory;
            }
    
        int munmap() 
            {
                int result = ::munmap( memory, sz ) ; 
            
                if( result != 0) 
                { 
                    success    = false; 
                    last_error = "Error: munmap has failed with error code : " + errno ;
                
                    return result ; 
                }
            
                memory = nullptr; 
                return result;
            }
    
    
        int close() 
            {
                int result = ::close( fd );
                fd        = -1 ;
                return result ;
            }     
    };

    // need to determine growth - compile_time - then is initial size etc. 
    template<size_t SZ = traits::default_size() >
    struct fs_traits : public traits
    {
        static int    default_size()         { return  SZ; }
            
        fs_traits(  string n     = traits::default_fname() ,
                    size_t s     = SZ,
                    int    f     = traits::default_flags() ,
                    mode_t perms = traits::default_permissions() )
        : traits(n,s,f,perms )
            {}
        };
    

    
    template<size_t SZ = traits::default_size() >
    struct fs_traits_ro : public fs_traits<SZ>
    {
        // TODO - size is an issue here
        //        should check the filesize really - not the compiled
        //        values
        
        static int    default_flags()        { return  O_RDONLY ; }
        
        fs_traits_ro(  string n     = fs_traits<SZ>::default_fname() ,
                       size_t s     = SZ,
                       int    f     = fs_traits_ro<SZ>::default_flags() ,
                       mode_t perms = fs_traits<SZ>::default_permissions() )
        : fs_traits<SZ>(n,s,f,perms )
        {
            assert( this -> oflags  == O_RDONLY ) ;

            this -> truncate = false ;
            this -> pflags   = PROT_READ ; 
        }
    };


    // need to determine growth - compile_time - then is initial size etc. 
    template<size_t SZ = traits::default_size() >
    struct shm_traits : public traits
    {
        static int    default_size()         { return  SZ; }
        static int    default_flags()        { return  O_CREAT | O_RDWR; } // { return  O_CREAT | O_EXCL | O_RDWR; }
        static string default_fname()        { return  "backing_store.bin" ; }

        shm_traits(  string n     = shm_traits<SZ>::default_fname() ,
                     size_t s     = SZ,
                     int    f     = shm_traits<SZ>::default_flags() ,
                     mode_t perms = shm_traits<SZ>::default_permissions() )
        : traits(n,s,f,perms )
        {
            assert( this -> oflags       == O_CREAT | O_RDWR  ) ;
            assert( this -> permissions  == S_IRUSR | S_IWUSR | S_IRGRP ) ;

            this -> pflags    = PROT_WRITE | PROT_READ ;
            this -> mflags    = MAP_SHARED | MAP_LOCKED | MAP_POPULATE ; 
            this -> truncate  = true ;
            this -> open_func = ::shm_open; 
        }
    };

        // need to determine growth - compile_time - then is initial size etc. 
    struct shm_traits_ro : public shm_traits<0>
    {
        static int    default_flags()        { return  O_RDWR; } // { return  O_CREAT | O_EXCL | O_RDWR; }

        shm_traits_ro(  string n     = shm_traits<0>::default_fname() ,
                        size_t s     = 0,
                        int    f     = shm_traits_ro::default_flags() ,
                        mode_t perms = shm_traits<0>::default_permissions() )
        : shm_traits<0>(n,s,f,perms)
        {
            this -> pflags    = PROT_READ ;
        }

    };
    





    
    template<typename T = traits >
    class store
    {
    private:

        T traits_;
        
        // TODO  - decide who owns the mapped memory pointer
        //         should this class own the void *
        //         I dont think this class should be templated by T
        //         that shoudl be the domain of the allocator
        
        
    public:
        

    // TODO - this should take traits class as optional - not individial parameter
    // TODO - shm impl -then perhaps fns move to store and out of traits
    // TODO - reserve???? something needs to have the wr_ptr functionality -
    //      - and now when crossing boundary
        store(  const T& t = T() ) 
    : traits_( t.fname, t.sz, t.oflags, t.permissions )
    {
    }

    // static??? - all of it could be static/templated..... 
    
    const string& last_error() const { return traits_.last_error() ; }
    
    bool operator!() const 
    {
        return ! traits_.success ; 
    }
  private:
    int open()    { return traits_.open() ; }
    void *mmap()  { return traits_.mmap(); }
  public:
    
    void *allocate() // templatize return???
    {
        open() ; 
    
        if( ! traits_.success ) return nullptr; // MAP_FAILED

        return mmap() ; 
    }

    void *allocate_next_block() 
    {
        // TODO - return pair?? first block, followed by current block?
        //      - contigous??? i think so w/mmap
        //      - do you need to re-mmap after you have ftruncated a second time?
        
        return traits_.allocate_next_block() ; 
    }
    
    };
    
    }; // ns allocator
    
}; // ns util


#endif


