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
#include <util/backing_store.h>



using namespace std;

int _file_size() 
{
  streampos begin,end;

  using util::backing_store::store;
  using util::backing_store::traits;



  ifstream myfile ( traits::default_fname().c_str(), ios::binary);
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


int _binary_create_backingstore_orig() 
{
  int i;
  int fd;
  int result;
  int *map;  /* mmapped array of int's */

  /* Open a file for writing.
   *  - Creating the file if it doesn't exist.
   *  - Truncating it to 0 size if it already exists. (not really needed)
   *
   * Note: "O_WRONLY" mode is not sufficient when mmaping.
   */
  fd = open( "/tmp/backing_store.bin", O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
  if (fd == -1)
    {
      perror("Error opening file for writing");
      exit(EXIT_FAILURE);
    }

  /* Stretch the file size to the size of the (mmapped) array of ints
   */
  result = lseek(fd, FILESIZE-1, SEEK_SET);
  if (result == -1)
    {
      close(fd);
      perror("Error calling lseek() to 'stretch' the file");
      exit(EXIT_FAILURE);
    }

  /* Something needs to be written at the end of the file to
   * have the file actually have the new size.
   * Just writing an empty string at the current file position will do.
   *
   * Note:
   *  - The current position in the file is at the end of the stretched
   *    file due to the call to lseek().
   *  - An empty string is actually a single '\0' character, so a zero-byte
   *    will be written at the last byte of the file.
   */
  result = write(fd, "", 1);
  if (result != 1)
    {
      close(fd);
      perror("Error writing last byte of the file");
      exit(EXIT_FAILURE);
    }

  /* Now the file is ready to be mmapped.
   */
  map = (int *) mmap(0, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (map == MAP_FAILED)
    {
      close(fd);
      perror("Error mmapping the file");
      exit(EXIT_FAILURE);
    }

  /* Now write int's to the file as if it were memory (an array of ints).
   */
  for (i = 1; i <=NUMINTS; ++i)
    {
      map[i] = 2 * i;
    }

  /* Don't forget to free the mmapped memory
   */
  if (munmap(map, FILESIZE) == -1)
    {
      perror("Error un-mmapping the file");
      /* Decide here whether to close(fd) and exit() or not. Depends... */
    }

  /* Un-mmaping doesn't close the file, so we still need to do that.
   */
  close(fd);
  return 0;
}



int _binary_create_backingstore_non_T() 
{
    
    using util::backing_store::store;
    using util::backing_store::traits;
    using util::backing_store::fs_traits_ro;
    using util::backing_store::fs_traits;


    // traits t;
    store< fs_traits<FILESIZE>> bs; // ( FILEPATH,FILESIZE );

    int *map = (int *) bs.allocate() ; 
    
    if (map == nullptr)
    {
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }
    
    /* Now write int's to the file as if it were memory (an array of ints).
     */
    for (auto i = 0; i < NUMINTS; ++i)
    {
        map[i] = 2 * i ;
    }
    
    // clean up now occurs via dtr of the backing_store object
    return 0;
} 



int _binary_read_backingstore_orig()
{
    int i;
    int fd;
    int *map;  /* mmapped array of int's */
    
    fd = open( "/tmp/backing_store.bin", O_RDONLY);     // FILEPATH, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    size_t abc = FILESIZE;
    map = (int *) mmap(0, abc, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        close(fd);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }
    
    /* Read the file int-by-int from the mmap
     */
    for (i = 0; i <NUMINTS; ++i) {
        printf("%d: %d\n", i, map[i]);
    }
    
    if (munmap(map, FILESIZE) == -1) {
        perror("Error un-mmapping the file");
    }
    close(fd);
    return 0;
}




int _binary_read_backingstore_non_T()
{
    using util::backing_store::store;
    using util::backing_store::fs_traits_ro;
    

    store< fs_traits_ro<FILESIZE>> bs; // ( FILEPATH,FILESIZE );

    int *map = (int *) bs.allocate() ; 
    
    if (map == nullptr )
    {
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }

    /* Read the file int-by-int from the mmap
     */
    for (auto i = 1; i <=NUMINTS; ++i) if( i == 1 || i % 50 == 0 )
                                           printf("%d: %d\n", i, map[i]);

    
    return 0;
}

template<typename T>
int _binary_read_backingstore( const char *tag )
{
    using util::backing_store::store;
    using util::backing_store::shm_traits_ro;
    

    store<T> bs;

    int *map = (int *) bs.allocate() ; 
    
    if (map == nullptr )
    {
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }

    /* Read the file int-by-int from the mmap
     */
    for (auto i = 0; i <NUMINTS; ++i) if( i == 0 || i % 50 == 0 || i == NUMINTS -1 )
                                          printf("%s %d: %d\n", tag, i, map[i]);

    
    return 0;
}

template<typename T>
int _binary_create_backingstore() 
{
    
    using util::backing_store::store;

    store<T> bs; 

    int *map = (int *) bs.allocate() ; 
    
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

int main () 
{
    // cerr << "GIT_VERSION: " << GIT_VERSION << endl;
    //_binary_create_backingstore() ;
    // _binary_read_backingstore() ;


     using util::backing_store::shm_traits;
     using util::backing_store::shm_traits_ro;

     using util::backing_store::fs_traits;
     using util::backing_store::fs_traits_ro;
     
     _binary_create_backingstore<fs_traits<FILESIZE>>() ;
     _binary_read_backingstore<fs_traits_ro<FILESIZE>>( "fs" ) ;


     _binary_create_backingstore<shm_traits<FILESIZE>>() ;
     _binary_read_backingstore<shm_traits_ro>( "sm" ) ;


     shm_traits<4> smt ; 
     assert( smt.truncate == true );

     fs_traits_ro<4> fsro ; 
     assert( fsro.truncate == false );
    //_txt() ;
    _file_size();
    return 0;
}


