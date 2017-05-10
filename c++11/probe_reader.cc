#include <util/backing_store.h>
#include <util/memory.h>
#include <iostream>
#include <fstream>
// Shared memory
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <vector>
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
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <cstdlib>        // std::rand

#include <util/probe.h>


using namespace std;
using namespace util;

using util::memory::allocator ;     
using util::backing_store::fs_traits_ro;
using util::probe::stats;

int main (int argc, char **argv ) 
{
    bool process_files      = false ;
    ++argv;

    vector<string> files;
    while(argc > 0 && *argv )
    {
        string arg =  *argv ;
        if( arg == "-files"  ) process_files = true ; 
        if( arg != "-files" && process_files ) files.push_back( arg ) ; 
        
        --argc ;
        ++argv ;
    }

    if( files.size() == 0 || process_files == false )
      {
        std::cerr << " -files filename[s] required." << std::endl;
        exit( 1 ) ;
      }
    
    std::string f = files[0] ;
    
    util::memory::allocator<stats,fs_traits_ro> l_allocator( f )  ;
    
    stats *probe_stats = l_allocator.allocate() ;

    assert( probe_stats != nullptr ) ; 
    
    // need to read header information and get the number of objects

    for( auto i = 0 ; i < l_allocator.size()  ; ++i )
      {
        probe_stats -> report() ;
        ++probe_stats ; 
      }
    return 0;
}


 
