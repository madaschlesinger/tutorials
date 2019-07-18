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
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cassert>
#include <libconfig.h++>


using namespace std;
using namespace libconfig;

bool parse_global_scope_( Setting& scope )
{
  assert( scope.getType()  == Setting::TypeGroup ) ;
  for( auto iter = scope.begin() ; iter != scope.end() ;  ++ iter )
    {
      Setting&    attribute = *iter ;
      std::string name      = iter -> getName() ; 
      if(      attribute.getType() == Setting::TypeString  ) cerr << '\t'<< name << ":" << attribute.c_str() << endl ;
      else if( attribute.getType() == Setting::TypeBoolean ) cerr << '\t'<< name << ":" << (bool)  attribute << endl ;
      else cerr << "\tname......" << endl;
    }
  
  return true;
}

bool parse_probe_config_( Setting& probe )
{
  assert( probe.getType()  == Setting::TypeGroup ) ;

  std::string       name, typeS ;
  bool              active = true ;
  double            warn   = std::numeric_limits<double>::max() ;
  double            error  = std::numeric_limits<double>::max() ;
  bool              quiet  = false ; // ??? log ???
  util::probe::type type   = util::probe::type::VALUE ;
  
  if( ! probe.exists( "name"   ) || ! probe.lookupValue( "name",   name )  )
    {
      cerr << '\t'<< "Invalid probe config entry - missing required: name   :"  << endl ;
      return false ; 
    }
  
  if( probe.lookupValue( "active",   active )) cerr << '\t'<< "active :" << active << endl ;
  if( probe.lookupValue( "queit",    quiet  )) cerr << '\t'<< "quiet  :" << quiet  << endl ;

  if( probe.exists( "type" ) && probe.lookupValue( "type",   typeS )  )
    {
      if(      typeS == "duration"  ) type = util::probe::type::DURATION ;
      else if( typeS == "occurence" ) type = util::probe::type::OCCURRENCE;
      else if( typeS == "value"     ) type = util::probe::type::VALUE ;
      else
        {
          cerr << '\t'<< "Invalid probe config entry - missing required: name   :"  << endl ;
          return false ;
        }

      return true ; 
    }
  



  
  return true;
}

bool parse_probe_scope_( Setting& scope )
{
  assert( scope.getType()  == Setting::TypeList || scope.getType()  == Setting::TypeArray ) ;

  for( auto iter = scope.begin() ; iter != scope.end() ;  ++ iter )
    {
      parse_probe_config_( *iter ) ; 
    }
  

  
  return true;
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


    char *probe_config_path = getenv( "PROBE_CONFIG_PATH" ) ; 

    Config cfg;
    
    // Read the file. If there is an error, report it and exit.
    try
      {
        cfg.readFile("./conf/probe.cfg");
      }
    catch(const FileIOException &fioex)
      {
        std::cerr << "I/O error while reading file." << std::endl;
        return(EXIT_FAILURE);
      }
    catch(const ParseException &pex)
      {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                  << " - " << pex.getError() << std::endl;
        return(EXIT_FAILURE);
      }

    // Find the 'movies' setting. Add intermediate settings if they don't yet
    // exist.
    Setting &root = cfg.getRoot();


    for( auto iter = root.begin() ; iter != root.end() ;  ++ iter )
      {
        Setting&    scope = *iter ;
        std::string name  = iter -> getName() ; 
        std::cerr << name << ":" << endl ;

        if( (std::string) scope.getName() == std::string( "global" ) ) parse_global_scope_( scope ) ;
        else                              parse_probe_scope_(  scope ) ; 

      }

    return(EXIT_SUCCESS);
}

