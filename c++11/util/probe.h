#ifndef __probe_h__
#define __probe_h__


#include <util/backing_store.h>
#include <string>
#include <chrono>
#include <limits>

namespace util { namespace probe {

        // these are designed to be singleton...
        class registry         ; 
        class config           ;
        class allocator        ; 

        
        class registry_entry   ; 
        class config_entry     ; 

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
            double    n_    ;
            
            time_point min_time_ ; 
            time_point max_time_ ; 

            std::function<void(stats *,double,time_point)> recorder_ ;

          public:

            // NOTE - when used in conjunction w/probe framework
            //      - usually this is palcement new - so no ctr is called
            stats( type t = probe::type::VALUE )
                {
                    t_     = t;
                    value_ = mean_ = std_ = n_ = 0 ;
                    max_   = std::numeric_limits<double>::min() ;  
                    min_   = std::numeric_limits<double>::max() ; 

                    abort() ; 
                }
            
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
                    
                    typename T::duration since_epoch = time.time_since_epoch();
                    seconds secs = duration_cast<seconds>(since_epoch);
                    since_epoch -= secs;
                    milliseconds milli = duration_cast<milliseconds>(since_epoch);
                    
                    s << '[' << sRep << ":" << milli.count() << "]\n";

                    return s; 
                }
            
            void report() const 
                {
                    
                    if( t_ == type::DURATION ) 
                    {
                        // TODO - think about us/vs ms in display - Duration/Clock - how should this be done...
                        std::cout << "duration: "
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
                        std::cout << "occurrence: "
                                  << n_
                                  << std::endl;
                    }
                    else if( t_ == type::VALUE ) 
                    {
                        std::cout << "value: "
                                  << value_
                                  << "us.\n";
                    }
                }
      




              
        }; // stats
            
        class observation  // ??? observation measure ??? recorder?, measurement? - sampling probe... traits???
        {
          protected:
            std::string    name_  ;
            stats *        stats_ ; 
            config_entry * cfg_   ;
            
          public:

            observation( std::string name, type probetype = type::VALUE )
                : name_( name )
                {
                    // config_entry - for now is null ;
                    cfg_ = nullptr ;
                    
                    // allocate stats ptr - uses placement new - no ctr is called
                    using util::memory::allocator ; 
                    static allocator<stats> s_allocator ;

                    stats_ = s_allocator.allocate() ;
                    stats_ -> name( name_ ) ; 
                    stats_ -> probe_type(  probetype ) ; 
                    
                    stats_ -> value_ = stats_ -> mean_ = stats_ -> std_ = stats_ -> n_ = 0 ;
                    stats_ -> max_   = std::numeric_limits<double>::min() ;  
                    stats_ -> min_   = std::numeric_limits<double>::max() ; 

                    double tt = std::numeric_limits<double>::max() ; 
                    if( stats_ -> min_   != tt  ) 
                        abort() ;  


                    // register.....
                }

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

            virtual void record() 
                {
                    // min/max/ min_time / max_time and other stats dont mean much w/an occurence probe
                    stats_ -> value_ += 1 ; 
                    stats_ -> max_    = stats_ -> value_  ;
                    stats_ -> n_      = stats_ -> value_  ;

                    // TODO - have lambda as part of stats recorder.... - 
                    //        avoids all the friends - better encapsulation ( maybe )
                }
        } ; 



        class duration : public observation
        {
          protected:

            std::chrono::system_clock::time_point start_ ; 
            std::chrono::system_clock::time_point end_   ;
 
          public:
            
            duration( const std::string& name )
                : observation( name, type::DURATION )
                {
                    std::function<void(stats *,double,std::chrono::system_clock::time_point)> f = duration::recorder; 
                    
                    // stats_ -> recorder_ = f ;  // set_recorder( f  ) ; 
                }

            std::chrono::system_clock::time_point start() 
                {
                    start_ = std::chrono::system_clock::now();
                    return start_ ; 
                }

            std::chrono::system_clock::time_point end() 
                {
                    end_ = std::chrono::system_clock::now();
                    return end_ ; 
                }

            static void recorder( stats *s, 
                                  const double& v, 
                                  const std::chrono::system_clock::time_point& t = std::chrono::system_clock::time_point::min() )
                {
                    s -> value_  = v ; 
                    
                    s -> mean_   = (( s -> mean_ * s -> n_ ) + s -> value_ ) / ( s -> n_ + 1 ) ;   
                    s -> n_     += 1;
                    
                    if( s -> value_ > s -> max_ ) 
                    {
                        s -> max_      = s -> value_ ; 
                        if( t != std::chrono::system_clock::time_point::min() ) s -> max_time_ = t ; 
                    }
                    
                    if( s -> value_ < s -> min_ ) 
                    {
                        s -> min_      = s -> value_ ; 
                        if( t != std::chrono::system_clock::time_point::min() ) s -> min_time_ = t ; 
                    }

                }

            virtual void record() 
                {
                    if( end_ < start_ ) end_  = std::chrono::system_clock::now();
                    uint32_t usec = std::chrono::duration_cast<std::chrono::microseconds>(end_ - start_).count() ; 

                    duration::recorder( stats_ , usec, end_ ); // might be faster to do this w/o the machinery... 
                }
        } ; 

        
    }; // ns probe
    
}; // ns util


#endif


