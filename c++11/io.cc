// obtaining file size
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

int _file_size() 
{
  streampos begin,end;
  ifstream myfile ("example.bin", ios::binary);
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

#include <cstdint>

namespace util { namespace allocator {

  enum class backing_store_type : std::int8_t { SHARED_MEMORY = 1, FILE_SYSTEM =2, MEMORY = 3  };

  int bac;

  // error handling...... throw/compile - template arg....??
template<typename T> 
class backing_store
{
private:

  T       mode_;
  int     fd_;
  int     flags_ = O_RDWR | O_CREAT | O_TRUNC;
  string  fname_;
  size_t  sz_; // TODO - need to think about block size -growth, pages/hugepages
  mode_t  perms_ = (mode_t) 0600;


public:
  
  backing_store( string fname, size_t sz, int flags= 0 , int perms = 0600 ) 
    : fname_( fname ), sz_( sz ), flags_( flags ) 
  {
    perms_ = (mode_t) perms ; 
  }

  mode_t default_flags() { return  O_RDWR | O_CREAT | O_TRUNC; }

  int open()
  {
    fd_ = open( fname_.c_str(), flags_, perms_ );
 
    if( fd_ == -1 )
    {
      perror("Error opening file for writing");
      exit(EXIT_FAILURE);
    }

    if( sz_ ) 
    {
      
      if(ftruncate( fd_ , sz_ ) != 0)
      {
	cout << " - ftruncate has failed with error code : " << errno << endl;
	exit errno;
      }
    }
    
    return fd_; // ??? keeping the open symantic - but breaks encapsulation
  }


};

}; // ns allocator

}; // ns util


int _binary_create_backingstore() 
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
  fd = open(FILEPATH, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0600);
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




int _binary_read_backingstore()
{
  int i;
  int fd;
  int *map;  /* mmapped array of int's */

  fd = open(FILEPATH, O_RDONLY);
  if (fd == -1)
    {
      perror("Error opening file for reading");
      exit(EXIT_FAILURE);
    }

  map = (int *) mmap(0, FILESIZE, PROT_READ, MAP_SHARED, fd, 0);
  if (map == MAP_FAILED) {
    close(fd);
    perror("Error mmapping the file");
    exit(EXIT_FAILURE);
  }

  /* Read the file int-by-int from the mmap
   */
  for (i = 1; i <=NUMINTS; ++i) {
    printf("%d: %d\n", i, map[i]);
  }

  if (munmap(map, FILESIZE) == -1) {
    perror("Error un-mmapping the file");
  }
  close(fd);
  return 0;
}


int main () 
{
  // _binary_create_backingstore() ;
  _binary_read_backingstore() ;
  _txt() ;
  _file_size();
  return 0;
}


