#include <iostream>
#include <string>
#include <cstdlib>

// Shared memory
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/types.h>
#include <string>

// Gestion des erreurs
#include <errno.h>

using namespace std;

int main(int argc, const char *argv[])
{
  if( argc == 0 )
    {
      cerr 	<<"Usage: " << endl
		<< '\t' << "bin/shm -action create|read|delete -value [ integer value to be written to file ] "
		<< endl;
      
      return -1;
    }
  
  string command, sval, fname("test25");
  int *result = NULL;
  int instance_id = 111; // not used - but from example code
      
  ++argv;

  while(argc > 0 && *argv )
    {
      string arg =  *argv ;
      if( arg == "-action" ) { command = *++argv ; argc -= 2; }
      if( arg == "-value"  ) { sval    = *++argv ; argc -= 2; }
      if( arg == "-fname"  ) { fname   = *++argv ; argc -= 2; }
      if( arg == "-create" ) { command = "create"; argc -= 1; }
      if( arg == "-delete" ) { command = "delete"; argc -= 1; }
      if( arg == "-read"   ) { command = "read"  ; argc -= 1; }

      ++argv ;
    }
  

  char filename[512] ; 
  sprintf( filename, "/%s", fname.c_str() ); 

  if (command == "create")
    {
      int    value   = atoi(sval.c_str());
      
      // For this example we are only writing out an integer 
      int integerSize = sizeof(int);
      // Open the shared memory - "manager"
      int descriptor = shm_open( filename, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
      if (errno == EEXIST) {
	cout << "Instance : " << instance_id << " - The exchange file 'test25' already exists" << endl;
      }
      if (descriptor == -1) {
	cout << "Instance : " << instance_id << " has failed." << endl;
	exit errno;
      }
      /* Size up the shared memory. */
      if (ftruncate(descriptor, integerSize) != 0){
	cout << "Instance : " << instance_id << " - ftruncate has failed with error code : " << errno << endl;
	exit errno;
      }
      result = (int*) mmap(NULL, integerSize, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_LOCKED | MAP_POPULATE, descriptor, 0 );
      if (result == MAP_FAILED) {
	cout << "Instance : " << instance_id << " - mmap failed with error code : " << errno << endl;
	exit errno;
      }
      // Init the shared memory
      *result = value;   // 161803;
      
      // clean
      if (munmap(result, sizeof(int)) != 0) {
			cout << "Instance : " << instance_id<< " - munmap has failed with error code : " << errno << endl;
			exit errno;
      }
    }
  if( command == "delete" )
    {
      
      if (shm_unlink( filename ) != 0) {
	cout << "Instance : " << instance_id << " - shm_unlink has failed with error code : " << errno << endl;
	exit errno;
      }
    }
  if( command == "read" )
    {
      //	case 26 : { // Test shared memory with shm_open & mmap
      int *result = NULL;
      int integerSize = sizeof(int);
      // Open the shared memory - "user"
      int descriptor = shm_open( filename, O_RDONLY, S_IRUSR);
      if (errno == ENOENT) {
	cout << "Instance : " << instance_id << " - The exchange file test25 does not exist" << endl;
      }
      if (descriptor == -1) {
	cout << "Instance : " << instance_id << " has failed." << endl;
	exit errno;
      }
      result = (int*) mmap(NULL, integerSize, PROT_READ, MAP_SHARED, descriptor, 0 );
      if (result == MAP_FAILED) {
	cout << "Instance : " << instance_id << " - mmap failed with error code : " << errno << endl;
	exit errno;
      }
      cout << "Instance_Id = " << *result << endl;
      if (munmap(result, sizeof(int)) != 0) { // unnecessary because the program exits - cf http://www.advancedlinuxprogramming.com/alp-folder/alp-ch05-ipc.pdf
	cout << "Instance : " << instance_id << " - munmap has failed with error code : " << errno << endl;
	exit errno;
      }
      // shm_unlink("/test25"); // As I undersqtand it, that should be performed only by the "manager"
    }
  
  return 0;
}
