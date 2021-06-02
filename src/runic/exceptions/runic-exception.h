#ifndef RUNIC_EXCEPTION_H
#define RUNIC_EXCEPTION_H

#ifdef RUNIC_PLATFORM_WINDOWS
  #ifdef RUNIC_EXCEPTION_IMPLEMENTATION
    #define RUNIC_EXCEPTION_API __declspec(dllexport)
  #else
    #define RUNIC_EXCEPTION_API __declspec(dllimport)
  #endif
#else
  #define RUNIC_EXCEPTION_API
#endif

#include <string>

using namespace std;

struct RUNIC_EXCEPTION_API RunicException {
  string prefix;
  string message;
  
  string get_message();
};

#endif
