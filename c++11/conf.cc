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

    static const char *output_file = "./conf/updated.cfg";

    Config cfg;
    
    // Read the file. If there is an error, report it and exit.
    try
      {
        cfg.readFile("./conf/example.cfg");
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

    if(! root.exists("inventory"))
      root.add("inventory", Setting::TypeGroup);

    Setting &inventory = root["inventory"];


    Setting& books       = root[ "inventory" ]["books"] ;
    Setting& book0       = root[ "inventory" ]["books"][0] ;
    int      book0_qty   = root[ "inventory" ]["books"][0]["qty" ] ; 

    assert( book0_qty == 5 ) ; 
    assert( book0.lookupValue( "qty", book0_qty )  == true ) ; 
    assert( book0_qty == 5 ) ; 

    
    if(! inventory.exists("movies"))
      inventory.add("movies", Setting::TypeList);

    
    Setting &movies = inventory["movies"];
    assert( movies.getType()  == Setting::TypeList ) ; 
    assert( movies.isGroup()  == false ) ;
    assert( movies.isScalar() == false ) ;
    assert( movies.isList()   == true ) ;
    assert( movies.isArray()  == false ) ;

    
    // Create the new movie entry.
    Setting &movie = movies.add(Setting::TypeGroup);

    assert( movie.getType()  == Setting::TypeGroup ) ; 
    assert( movie.isGroup()  == true  ) ;
    assert( movie.isScalar() == false ) ;
    assert( movie.isList()   == false ) ;
    assert( movie.isArray()  == false ) ;
            
    movie.add("title", Setting::TypeString) = "Buckaroo Banzai";
    movie.add("media", Setting::TypeString) = "DVD";
    movie.add("price", Setting::TypeFloat) = 12.99;
    movie.add("qty", Setting::TypeInt) = 20;

    std::string bb = movie[ "title" ] ;
    //decltype( movie[ "title" ] ) abc;  bb.c_str()  ;
    for( auto iter = movie.begin() ; iter != movie.end() ;  ++ iter )
      {
        Setting& attribute = *iter ;
        std::string value ; // = (std::string) attribute.c_str() ; 
        assert( iter -> isScalar() == true ) ; 
        std::string name = iter -> getName() ; 
        std::cerr << name << ":" ;

        if( iter -> getType() == Setting::TypeString )
        {
          std::string value = attribute ; 
          std::cerr << value ;
        }
        else if( iter -> getType() == Setting::TypeFloat )
        {
          double value = attribute ; 
          std::cerr << value ;
        }
        else if( iter -> getType() == Setting::TypeInt )
        {
          int value = attribute ; 
          std::cerr << value ;
        }
        else
          std::cerr << "wtf type???" << endl;
          

        std::cerr << std::endl;
      }

    std::string dvd;
    movie.lookupValue( "media", dvd ) ; 



 try
  {
    const Setting &books = root["inventory"]["books"];
    int count = books.getLength();

    cout << setw(30) << left << "TITLE" << "  "
         << setw(30) << left << "AUTHOR" << "   "
         << setw(6) << left << "PRICE" << "  "
         << "QTY"
         << endl;

    for(int i = 0; i < count; ++i)
    {
      const Setting &book = books[i];

      // Only output the record if all of the expected fields are present.
      string title, author;
      double price;
      int qty;

      if(!(book.lookupValue("title", title)
           && book.lookupValue("author", author)
           && book.lookupValue("price", price)
           && book.lookupValue("qty", qty)))
        continue;

      cout << setw(30) << left << title << "  "
           << setw(30) << left << author << "  "
           << '$' << setw(6) << right << price << "  "
           << qty
           << endl;
    }
    cout << endl;
  }
  catch(const SettingNotFoundException &nfex)
  {
    // Ignore.
  }





    
    // Write out the updated configuration.






    
    try
      {
        cfg.writeFile(output_file);
        cerr << "Updated configuration successfully written to: " << output_file
             << endl;

      }
    catch(const FileIOException &fioex)
      {
        cerr << "I/O error while writing file: " << output_file << endl;
        return(EXIT_FAILURE);
      }

    return(EXIT_SUCCESS);
}

