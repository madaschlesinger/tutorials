#ifndef __probe_h__
#define __probe_h__


#include <util/memory.h>
#include <util/backing_store.h>
#include <util/singleton.h>
#include <string>
#include <chrono>
#include <limits>
#include <map>
#include <string>
#include <thread>
#include <mutex>

namespace util { namespace probe {

    enum class type : std::int8_t
    {
      VALUE      = 1,
        OCCURRENCE = 2,
        DURATION   = 3
        };
        
    // explicitly struct - as most often used in placement new - w/no ctr - so not 'class-ish'
    //                   - ignoring the struct/class comments
    struct stats
    {
      // TODO - need to think about the Clock/Duration
      //        in the context of the abcking store - and the reader
      //        we would want only one type if possible...
      typedef std::chrono::system_clock::time_point time_point;  
          
      // friend class observation ; 
      // friend class occurrence  ; 
      // friend class duration    ; 
          
      type      t_       ; 
      char      name_[36];
      double    value_   ;
          
      double    min_  ; 
      double    max_  ;
      double    std_  ;
      double    mean_ ;
      uint32_t  n_    ;
          
      time_point min_time_ ; 
      time_point max_time_ ; 
          
      std::function<void(stats *,double,time_point)> recorder_ ;
          
          
    private:
      // NOTE - when used in conjunction w/probe framework
      //      - usually this is palcement new - so no ctr is called
      stats( type t = probe::type::VALUE )
      {
        abort() ; 
      }
          
    public:
      void   value( double d ) { value_ = d ; }
      double value() const     { return value_ ; }
          
      type   probe_type() const     { return t_ ; }
          
      void name( std::string n )
      {
        size_t sz = sizeof( name_ ) ; 
        strncpy( name_, n.c_str(), sz - 1 );
        name_[sz-1] = '\0' ; 
      }
          
      void probe_type( type t ) { t_ = t ; }
          
      void set_recorder( std::function<void(stats *,double,time_point)> f ) 
      { 
        recorder_ = f ; 
      }
          
      void record(double value, std::chrono::system_clock::time_point t = std::chrono::system_clock::time_point::min() )
      {
        ::abort() ; 
        recorder_( this, value, t ) ; 
      }
          
      template<typename T>
      std::ostream& print_time( std::ostream& s, std::chrono::time_point<T> time) const 
      {
        using namespace std;
        using namespace std::chrono;
            
        time_t curr_time = T::to_time_t(time);
        char sRep[100];
        strftime(sRep,sizeof(sRep),"%Y-%m-%d %H:%M:%S",localtime(&curr_time));
            
        typename T::duration since_epoch  = time.time_since_epoch();
        seconds secs                      = duration_cast<seconds>(since_epoch);
        since_epoch                      -= secs;
        milliseconds milli                = duration_cast<milliseconds>(since_epoch);
            
        s << '[' << sRep << ":" << milli.count() << "]\n";
            
        return s; 
      }
          
      void report() const 
      {
        // TODO - think about this - this needs to be cleaned up report<T>( stats ) of some form
        //        or an appropriate std::function more likely - this way we can custimize what
        //        a reporting infra looks like
        
        if( t_ == type::DURATION ) 
          {
            // TODO - think about us/vs ms in display - Duration/Clock - how should this be done...
            std::cout << "duration[ " << name_ << "]: "
                      << value_
                      << "us.\n";
                        
            std::cout << '\t' << "mean: "
                      << mean_
                      << "us.\n";
                        
            //const time_t mtime = std::chrono::system_clock::to_time_t(max_time_);
                        
            std::cout << '\t' << "min: "
                      << min_
                      << "us. min_time: "; 
            print_time( std::cout, min_time_ )  ; 
                        
            std::cout << '\t' << "max: "
                      << max_
                      << "us. max_time: " ;
            print_time( std::cout, max_time_ )  ; 

            std::cout << '\t' << "observations: "
                      << n_
                      << std::endl;
          }
        else if( t_ == type::OCCURRENCE ) 
          {
            std::cout << "occurrence[ " << name_ << "]: "
                      << n_
                      << std::endl;
          }
        else if( t_ == type::VALUE ) 
          {
            std::cout << "value[ " << name_ << "]: "
                      << value_
                      << std::endl;
          }
        else 
          {
            std::cout << "unknown probe type[ " << name_ << "]: "
                      << std::endl;
          }

      }
    }; // stats

    
    struct traits
    {
      static const bool auto_record       = true  ;
      static const bool is_sampling_probe = false ; 
      static const bool is_value_probe    = true  ; 

    };

    // these are designed to be singleton...


    class config_entry     ; 
    class config : public std::map< std::string, config_entry * >
    {
    public:
      typedef std::map<std::string, config_entry*> map;

      static bool initialize()
      {
        // TOOD read probe configuration - and populate config registry
        return true ;
      }
      
      std::pair<iterator, bool > register_probe_config( const std::string& name, config_entry *pc )
      {
        // thread saftey....
        static std::mutex s_registry_mutex ; 
        std::lock_guard<std::mutex> guard( s_registry_mutex);
        return insert( map::value_type( name , pc ) ) ;
      }

      static bool is_active( const std::string& name )
      {
        // TODO - look up in probe config - and see if the probe is active

        return true ; 
      }
    };
    


    
    class observation ;
    class registry : public std::map< std::string, observation * >
    {
    public:
      typedef std::map<std::string, observation*> map;
      
      std::pair<iterator, bool > register_probe( const std::string& name, observation *po )
      {
        // thread saftey....
        static std::mutex s_registry_mutex ; 
        std::lock_guard<std::mutex> guard( s_registry_mutex);
        return insert( std::map<std::string, observation*>::value_type( name , po ) ) ;
      }
    };
    
    class observation  // ??? observation measure ??? recorder?, measurement? - sampling probe... traits???
    {
    protected:
      std::string    name_     ;
      stats *        stats_    ; 
      config_entry * cfg_      ;

    public:

      observation( std::string name, type probetype = type::VALUE )
        : name_( name )
      {
        // config_entry - for now is null ;
        cfg_   = nullptr ;
        stats_ = nullptr ; 
        if( util::singleton<config>::instance().is_active( name ) )
          {
            // allocate stats ptr - uses placement new - no ctr is called
            using util::memory::allocator ; 
            // static allocator<stats> s_allocator ;
            typedef util::singleton< allocator<stats> > stats_allocator ;
            
            stats_ = stats_allocator::instance().allocate() ;
            
            stats_ -> name( name_ ) ; 
            stats_ -> probe_type(  probetype ) ; 
            
            stats_ -> value_ = stats_ -> mean_ = stats_ -> std_ = stats_ -> n_ = 0 ;
            stats_ -> max_   = std::numeric_limits<double>::min() ;  
            stats_ -> min_   = std::numeric_limits<double>::max() ; 
            
            // register.....
            util::singleton<registry>::instance().register_probe( name, this ) ;

            //TODO - get the config_entry ptr
            auto iter = util::singleton<config>::instance().find( name ) ;
            if( iter != util::singleton<config>::instance().end() ) cfg_ = iter ->second  ; 
              
          }
      }

      virtual ~observation()
      {}
      
      const double&       value()       const { return stats_ -> value_ ; }
      const uint32_t&     occurrences() const { return stats_ -> n_ ; }
      const std::string&  name()        const { return name_  ; }
      const stats *       statistics()  const { return stats_ ; }
      
      virtual void record() = 0 ;  
                
      virtual void report() const 
      {
        stats_ -> report() ; 
      }
    } ; 

    class occurrence : public observation
    {
    public:

      occurrence( const std::string& name )
        : observation( name, type::OCCURRENCE )
      {
      }

      ~occurrence()
      {
        record() ; 
      }

      virtual void record() 
      {
        // min/max/ min_time / max_time and other stats dont mean much w/an occurence probe
        stats_ -> value_ += 1 ; 
        stats_ -> max_    = stats_ -> value_  ;
        stats_ -> n_      = stats_ -> value_  ;
      }
    } ; 



    class duration : public observation
    {
    public:
 
      typedef std::chrono::high_resolution_clock::time_point time_point;
      typedef std::chrono::high_resolution_clock             clock ; 
   protected:

      time_point start_ ; // depends on impl - might be == system_clock
      time_point end_   ; // or steady_clock per doc
 
    public:
            
      duration( const std::string& name )
        : observation( name, type::DURATION )
      {
        this -> start() ; 
        end_ = time_point::min() ; 

      }


      ~duration()
      {
        record() ;
      }
            
      time_point start() 
      {
        start_ = clock::now();
        return start_ ; 
      }

      time_point end() 
      {
        end_ = clock::now();
        return end_ ; 
      }

      time_point time_point_start() 
      {
        return start_ ; 
      }

      time_point time_point_end() 
      {
        return end_ ; 
      }

      
      static void recorder( stats *s, 
                            const double& v, 
                            const time_point& t = time_point::min() )
      {
        s -> value_  = v ; 
                    
        s -> mean_   = (( s -> mean_ * s -> n_ ) + s -> value_ ) / ( s -> n_ + 1 ) ;   
        s -> n_     += 1;
                    
        if( s -> value_ > s -> max_ ) 
          {
            s -> max_      = s -> value_ ; 
            if( t != time_point::min() ) s -> max_time_ = t ; 
          }
                    
        if( s -> value_ < s -> min_ ) 
          {
            s -> min_      = s -> value_ ; 
            if( t != time_point::min() ) s -> min_time_ = t ; 
          }

      }

      virtual void record() 
      {
        if( end_ < start_ ) end_  = clock::now();
        uint32_t usec = std::chrono::duration_cast<std::chrono::microseconds>(end_ - start_).count() ; 

        duration::recorder( stats_ , usec, end_ ); // do we need the static fn...
      }

    } ;     
        
  }; // ns probe
    
}; // ns util


#endif


