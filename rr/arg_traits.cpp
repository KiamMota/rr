#include "arg_traits.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> arg_traits::find_exception_contents(int &argn, char **&argv)
{
  std::vector<std::string> excepctions;
  for(int i = 0; i<argn; i++)
  {
    if(std::strcmp(argv[i], "-e") == 0 || std::strcmp(argv[i], "--exceptions") == 0)
    {
      for(int j = i + 1; j<argn; j++)
      {
        if(argv[j][0] != '-')
          excepctions.push_back(argv[j]);
      }
    }
  }
  return excepctions;
}

bool arg_traits::find_recursive_flag(const int &argn, char **&argv)
{

  for(int i = 0; i<argn; i++)
  {
    if(std::strcmp(argv[i], "-r") == 0 || std::strcmp(argv[i], "--recursive") == 0)
      return true;
  }
  return false;
}

bool arg_traits::find_timestamp_flag(const int &argn, char **&argv)
{

  for(int i = 0; i<argn; i++)
  {
    if(std::strcmp(argv[i], "-t") == 0 || std::strcmp(argv[i], "--timestamp") == 0)
      return true;
  }
  return false;
}

bool arg_traits::find_verbosity_flag(const int &argn, char **&argv)
{

  for(int i = 0; i<argn; i++)
  {
    if(std::strcmp(argv[i], "-v") == 0 || std::strcmp(argv[i], "--verbosity") == 0)
    {
      std::cout << "starting rr with verbosity." << std::endl;
      return true;
    }

  }
  return false;
}
