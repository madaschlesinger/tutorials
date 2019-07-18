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
#include <vector>
#include <util/singleton.h>
#include <libconfig.h++>
#include <sstream>
#include <iostream>


// using namespace libconfig;

namespace util { namespace probe {

    using libconfig::Config;
    using libconfig::Setting;
    using libconfig::FileIOException ;
    using libconfig::ParseException  ;
    using std::vector;
    using std::cerr ; 
    using std::endl ; 
    
    
    bool config::parse_global_scope_( Setting& scope )
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

    bool config::parse_probe_config_( Setting& probe )
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

      cerr << '\t'<< "name   :" << name << endl ;
  
      if( probe.lookupValue( "active",   active )) cerr << '\t'<< "active :" << active << endl ;
      if( probe.lookupValue( "quiet",    quiet  )) cerr << '\t'<< "quiet  :" << quiet  << endl ;

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

      config_entry *ce = new config_entry{ name, type, active, quiet, warn, error } ; 

      auto result = singleton<config>::instance().register_probe_config( name, ce ) ;

      if( result.second == false )
        {
          // this should be a normal situation of an update - lets check the entry exists...
          // so we dont drop config entries silently
          const config_entry *pce = singleton<config>::instance().find_entry( name );

          bool ret_val = ( ce != pce ) && ( pce != nullptr ) ;

          delete ce ;

          if( ! ret_val ) cerr << '\t'<< "probe config processing error for name :" << name  << endl ;

          return ret_val ; 
        }
      
      return true;
    }

    bool config::parse_probe_scope_( Setting& scope )
    {
      assert( scope.getType()  == Setting::TypeList || scope.getType()  == Setting::TypeArray ) ;

      for( auto iter = scope.begin() ; iter != scope.end() ;  ++ iter )
        {
          parse_probe_config_( *iter ) ; 
        }
  

  
      return true;
    }


    std::vector<std::string> config::parse_path_(  const char *dirs )
    {
      const char *probe_config_path  = ( dirs != nullptr ) ? dirs : getenv( "PROBE_CONFIG_PATH" ) ;

      std::vector<std::string> paths ;
  
      if( ! probe_config_path )
        {
          paths.push_back( "."        ) ; 
          paths.push_back( "./conf"   ) ; 
          paths.push_back( "./config" ) ; 
          paths.push_back( "./cfg"    ) ; 

          return paths ;
        }
    }

    inline bool file_exists_(const std::string& name)
    {
      struct stat buffer;   
      return (stat (name.c_str(), &buffer) == 0); 
    }

    std::vector<std::string> config::parse_files_( const std::vector<std::string>& dirs, const char *files )
    {
      const char *probe_config_files = ( files != nullptr ) ? files : getenv( "PROBE_CONFIG_FILES" ) ;

      std::vector<std::string> filenames, qualified_files ;
    
      if( ! probe_config_files )
        {
          filenames.push_back( "probe.cfg" ) ; 
        }
      else
        {
          using std::string ; 
          std::istringstream f(files);
          string s;    
          while (getline(f, s, ':'))
            filenames.push_back(s);
        }
    
      for( auto f:filenames )
        for( auto d:dirs ) 
          {
            std::string file = d + "/" + f ; 
            if( file_exists_( file ) )
              {
                cerr << "file exists:" << file << endl;
                qualified_files.push_back( file ) ; 
                break ;
              }
          }

      return qualified_files ; 
    }
  


    bool config::read_configuration( const char *filenames ) 
    {
      std::vector<std::string> paths = parse_path_() ; 
      std::vector<std::string> files = parse_files_( paths, filenames          ) ; 
  
      if( ! files.size() )
        {
          std::cerr << "no probe_config files... " << std::endl;
          return false;
        }
  
      for( auto file: files )
        {
      
          std::cerr << "yaaaa mon probe_config files... " << file << std::endl;
          Config cfg;
      
          // Read the file. If there is an error, report it and exit.
          try
            {
              cfg.readFile( file.c_str() );
            }
          catch(const FileIOException &fioex)
            {
              std::cerr << "I/O error while reading file." << std::endl;
              return false ; 
            }
          catch(const ParseException &pex)
            {
              std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                        << " - " << pex.getError() << std::endl;
              return false ; 
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
              else                                                           parse_probe_scope_(  scope ) ; 
            }
        }
  
      return true ;     
    }
  } // namespace probe
} // namespace util
