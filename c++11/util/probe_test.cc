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
using namespace util;

using util::backing_store::fs_traits;
using util::probe::stats;
using util::probe::observation;
using util::probe::duration;
using util::probe::occurrence;

void _casserts()
{
  {
    probe::occurrence  p( "occurrence:assert:test:1" ) ;

    const probe::stats * stats = p.statistics() ;

    assert( stats -> min_    == std::numeric_limits<double>::max()  ) ; 
    assert( stats -> max_    == std::numeric_limits<double>::min()  ) ; 
    assert( stats -> value_  == 0 ) ; 
    assert( stats -> t_      == util::probe::type::OCCURRENCE ) ; 

    // stats_ -> value_ = stats_ -> mean_ = stats_ -> std_ = stats_ -> n_ = 0 ;
  }
  {
      probe::duration  d( "duration:assert:test:1" ) ;
      assert( d.time_point_end()  <  d.time_point_start() );
      assert( d.time_point_end()  == probe::duration::time_point::min() );
  }

}

void _occurrence_tests()
{
  
  probe::occurrence  p( "occurrence:test" ) ;
  
  for( auto i = 0 ; i < 10 ; ++i )  p.record()  ; 

  assert( p.occurrences() == 10 ) ; 

}

void _duration_tests()
{
  
  probe::duration  d( "duration:tests" ) ; 
  
  std::srand( std::time(0) );
  for( auto i = 0 ; i < 100 ; ++i )
    {
      d.start() ; 
      
      uint32_t msec = std::rand() % 10 ; 
      std::this_thread::sleep_for(std::chrono::milliseconds( msec ));
      
      d.end() ; 
      
      d.record() ; 
      
    }        
  
  d.report() ; 
}

void _probe_config_tests()
{
  using util::probe::config ;
  using util::probe::config_entry ;
  using util::singleton ; 

  
  bool result = singleton<config>::instance().initialize( "probe.cfg:probe_test.cfg" ) ; 

  assert( result == true ) ;
  
  config::iterator iter = singleton<config>::instance().find( "occurrence:assert:dup:na" ) ;

  assert( iter != singleton<config>::instance().end() ) ; 
  assert( iter -> second -> active == false ) ;

  config_entry *ce = singleton<config>::instance().find_entry( "occurrence:assert:dup:na" ) ;
  
  assert( ce == iter -> second ) ; 
  assert( ce != nullptr  ) ;

  probe::occurrence  p( "occurrence:assert:dup:na" ) ;

  assert( p.statistics() == nullptr ) ;

  p.record() ; // should be safe - even tho not active  
  
}


int main (int argc, char **argv ) 
{
  bool bs       = true ;
  bool alloc    = true ; 
  
  bool casserts = true ; 
  bool probes   = true ;
  bool cfg      = true ;
  ++argv;
  
  while(argc > 0 && *argv )
    {
      string arg =  *argv ;
      if( arg == "-alloc"   ) bs    = casserts = probes = cfg      = false ; 
      if( arg == "-bs"      ) alloc = casserts = probes = cfg      = false ; 
      if( arg == "-probes"  ) alloc = casserts = bs     = cfg      = false ; 
      if( arg == "-cassert" ) alloc = bs       = probes = cfg      = false ; 
      if( arg == "-config"  ) alloc = bs       = probes = casserts = false ; 
      
      --argc ;
      ++argv ;
    }
  
  _casserts() ;

  if( cfg )
    {
      _probe_config_tests() ; 
    }

  if( probes ) 
    {
      probe::duration  d( "duration:outer:tests" ) ; 
      _duration_tests()   ; 
      _occurrence_tests() ;
      
      d.record() ; 
      d.report() ; 
    }
  return 0;
}


 
