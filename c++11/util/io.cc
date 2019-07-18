#include <util/backing_store.h>
#include <util/memory.h>
#include <iostream>
#include <fstream>
// Shared memory
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/types.h>
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

#define FILEPATH "/tmp/mmapped.bin"
#define NUMINTS  (1000)
#define FILESIZE (NUMINTS * sizeof(int))




using namespace std;
using util::backing_store::shm_traits;
using util::backing_store::shm_traits_ro;

using util::backing_store::fs_traits;
using util::backing_store::fs_traits_ro;

using util::backing_store::store;
using util::backing_store::traits;




int _file_size() 
{
  streampos begin,end;

  ifstream myfile ( fs_traits::file_name( fs_traits::filename() ).c_str(), ios::binary);
  begin = myfile.tellg();
  myfile.seekg (0, ios::end);
  end = myfile.tellg();
  myfile.close();
  cout << "size is: " << (end-begin) << " bytes.\n";
  return 0;
}



int _txt() {
  ofstream myfile ("example.txt");
  if (myfile.is_open())
    {
      myfile << "This is a line.\n";
      myfile << "This is another line.\n";
      myfile.close();
    }
  else cout << "Unable to open file";
  return 0;
}


int _binary_read_contents() 
{
  streampos size;
  char * memblock;

  ifstream file ("example.bin", ios::in|ios::binary|ios::ate);
  if (file.is_open())
  {
      size = file.tellg();
      memblock = new char [size];
      file.seekg (0, ios::beg);
      file.read (memblock, size);
      file.close();
      
      cout << "the entire file content is in memory";
      
      delete[] memblock;
  }
  else cout << "Unable to open file";
  return 0;
}


template<typename T>
int _binary_read_backingstore( const char *tag )
{
    store<T> bs;

    int *map = (int *) bs.open_and_mmap() ; 
    
    if (map == nullptr )
    {
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }

    /* Read the file int-by-int from the mmap
     */
    for (auto i = 0; i <NUMINTS; ++i) if( i == 0 || i % 200 == 0 || i == NUMINTS -1 )
                                          printf("%s %d: %d\n", tag, i, map[i]);

    
    return 0;
}

template<typename T>
int _binary_create_backingstore() 
{
    
    store<T> bs; 

    int *map = (int *) bs.open_and_mmap() ; 
    
    if (map == nullptr)
    {
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }
    
    /* Now write int's to the file as if it were memory (an array of ints).
     */
    for (auto i = 0; i < NUMINTS; ++i)
    {
        map[i] = 2 * i  ;
    }
    
    // clean up now occurs via dtr of the backing_store object
    return 0;
} 

void   _casserts()
{
          
  
  assert( fs_traits::file_open_flags   == O_RDWR  | O_CREAT | O_TRUNC ) ;
  assert( fs_traits::file_permissions  == S_IRUSR | S_IWUSR | S_IRGRP ) ;
  assert( fs_traits::is_readonly       == false     ) ;
  assert( fs_traits::truncate          == true    ) ;
  assert( fs_traits::mmap_flags        == MAP_SHARED      ) ;
  assert( fs_traits::mmap_protection   == PROT_WRITE | PROT_READ    ) ;

  assert( fs_traits_ro::file_open_flags  == O_RDONLY  ) ;
  assert( fs_traits_ro::file_permissions == S_IRUSR | S_IWUSR | S_IRGRP ) ;
  assert( fs_traits_ro::is_readonly      == true      ) ;
  assert( fs_traits_ro::truncate         == false     ) ;
  assert( fs_traits_ro::mmap_flags       == MAP_SHARED      ) ;
  assert( fs_traits_ro::mmap_protection  == PROT_READ ) ;

  assert( shm_traits::file_open_flags   == O_CREAT | O_RDWR ) ;
  assert( shm_traits::file_permissions  == S_IRUSR | S_IWUSR | S_IRGRP ) ;
  assert( shm_traits::is_readonly       == false    ) ;
  assert( shm_traits::truncate          == true     ) ;
  assert( shm_traits::mmap_flags        == MAP_SHARED | MAP_LOCKED | MAP_POPULATE     ) ;
  assert( shm_traits::mmap_protection   == PROT_WRITE | PROT_READ    ) ;

  assert( shm_traits_ro::file_open_flags   == O_RDONLY ) ;
  assert( shm_traits_ro::file_permissions  == S_IRUSR | S_IWUSR | S_IRGRP ) ;
  assert( shm_traits_ro::is_readonly       == true   ) ;
  assert( shm_traits_ro::truncate          == false  ) ;
  assert( shm_traits_ro::mmap_flags        == MAP_SHARED | MAP_LOCKED | MAP_POPULATE     ) ;
  assert( shm_traits_ro::mmap_protection   == PROT_READ    ) ;

  shm_traits smt ; 
  assert( smt.truncate == true );
  
  fs_traits_ro fsro ; 
  assert( fsro.truncate == false );
  
}

void _backing_store()
{
  std::cerr << "fs backing store ***********************" << std::endl;
  _binary_create_backingstore<fs_traits>() ;
  _binary_read_backingstore<fs_traits_ro>( "fs" ) ;
  
  std::cerr << "shm backing store ***********************" << std::endl;
  
  _binary_create_backingstore<shm_traits>() ;
  std::cerr << "shm ro backing store ***********************" << std::endl;  
  _binary_read_backingstore<shm_traits_ro>( "sm" ) ;
}

void _allocator()
{
    using util::memory::allocator ; 

    allocator<int> a;
    
    allocator<int>::value_type abc = 0 ;
    
    bool tf = a.has_spare_capacity( 1 ) ;


    // allocators capacity - should be traits::default_capacity.  the stores capacity includes the header
    assert( a.capacity() == fs_traits::initial_capacity ) ; 
    
    assert( a.has_spare_capacity( 1    ) == true ) ; 
    assert( a.has_spare_capacity( 1023 ) == true ) ; 
    assert( a.has_spare_capacity( 1024 ) == true ) ; 

    int size_max = a.max_size() ; 
    assert( size_max == a.capacity() / sizeof( int ) ) ; 
    
    int *m = a.allocate( 1024 );
    int size_c   = a.size() ;

    assert( size_c ==  1024 ) ;     
    assert( a.has_spare_capacity( size_max -size_c    ) == true  ) ; 
    assert( a.has_spare_capacity( size_max -size_c +1 ) == false ) ; 

    
    for (auto i = 0; i < 1024; ++i)
    {
        m[i] = 2 * i  ;
    }

    {
        
        using util::backing_store::store;             
        store<fs_traits_ro> bs;
        _binary_read_backingstore<fs_traits_ro>( "fsa" ) ;
    }
    
    
    {
        allocator<int,fs_traits_ro> a;
        
        allocator<int>::value_type abc = 0 ;
        
        int *m = a.allocate( 1024 );
        
        for (auto i = 0; i < 1024; ++i)
        {
            assert( m[i] == 2 * i ) ;
        }
    }
    
    
#if 0 
    try {
        std::bad_alloc exception;
        throw ( exception ); 
    }
    catch (const std::bad_alloc&) {
        return -1;
    }
    
#endif
}


void _files()
{
    _txt() ;
    _file_size();
    
}

int main (int argc, char **argv ) 
{
    bool bs       = true ;
    bool alloc    = true ; 
    bool casserts = true ; 
    bool files    = true ; 
    ++argv;
    
    while(argc > 0 && *argv )
    {
        string arg =  *argv ;
        if( arg == "-alloc"   ) bs    = casserts = files = false ; 
        if( arg == "-bs"      ) alloc = casserts = files = false ; 
        if( arg == "-files"   ) alloc = casserts = bs    = false ; 
        if( arg == "-cassert" ) alloc = bs       = files = false ; 

        --argc ;
        ++argv ;
    }

    _casserts() ;
    
    if( bs       ) _backing_store() ;
    if( files    ) _files()  ; 
    if( alloc    ) _allocator() ; 

     return 0;
}


