#ifndef _RRARGTRAITS_HPP_
#define _RRARGTRAITS_HPP_

#include <string>
#include <vector>
namespace arg_traits
{
  std::vector<std::string> find_exception_contents(int& argn, char**& argv);
  bool find_verbosity_flag(int& argn, char**& argv);
  bool find_timestamp_flag(int& argn, char**&argv);
  bool find_recursive_flag(int& argn, char**& argv);

}

#endif
