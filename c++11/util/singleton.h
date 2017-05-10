#ifndef __singleton_h__
#define __singleton_h__

namespace util {

    template<typename T>
    class singleton
    {
    public:
      // as of c++11 - statics creation/initialization is thread safe
      // missing some template typedef - causing declaration problems...
      //
      static T& instance()
      {
        // c++11 - if compiler is supporitng the standard 100% - you dont need
        // std::once etc. - nor the double check stuff
        static T s_instance ;
        return s_instance ;
      }
    private:
      singleton() {}; // so we cannot accidentally delete it via pointers
      singleton(const singleton&)            = delete; // no copies
      singleton& operator=(const singleton&) = delete; // no self-assignments

      singleton(singleton&&)                 = delete; // should not need move del due to
      singleton& operator=(singleton&&)      = delete; // decl of user define cpy ctr
    };

}; // ns util


#endif
