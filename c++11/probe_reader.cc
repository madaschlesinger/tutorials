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
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <cstdlib>        // std::rand

#include <util/probe.h>


using namespace std;

using util::backing_store::fs_traits;
using util::probe::stats;
using util::probe::observation;
using util::probe::duration;
using util::probe::occurrence;

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

     // observation foo( "foo:bar" ) ; 

     occurrence  goo( "goo:bar" ) ;

     for( auto i = 0 ; i < 10 ; ++i )  goo.record()  ; 


     duration  d( "doo:bar" ) ; 

     std::srand( std::time(0) );
     for( auto i = 0 ; i < 10 ; ++i )
     {
         d.start() ; 
         
         uint32_t msec = std::rand() % 10 ; 
         std::this_thread::sleep_for(std::chrono::milliseconds( msec ));
         
         d.end() ; 
         
         d.record() ; 

     }        
 
     goo.report() ; 
     d.report() ; 

     return 0;
}


 
