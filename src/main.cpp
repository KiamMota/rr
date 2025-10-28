#include <cstring>
#include <iostream>
#include <filesystem>
#include <string>

#include "messages.hpp"
#include "arg_traits.hpp"
#include "file.hpp"

int main(int argn, char** argv)
{
  /* to skip the command name*/
  argv++;
  /* to sync with the argv */
  argn--;
  if(argn == 0)
  {
    messages::help();
    std::exit(0);
  }
  if(argn <= 2 && (std::strcmp(argv[0], "-v") == 0 || std::strcmp(argv[0], "--version") == 0))
  {
    messages::version();
    std::exit(0);
  }

  if(argn <= 2)
  {
    messages::help();
    std::exit(0);
  }

  /* parsing arguments */

  std::string old_word = argv[0];
  std::string new_word = argv[1];
  std::string context = argv[2];
  
  bool is_with_timestamp = arg_traits::find_timestamp_flag(argn, argv);
  bool is_verbosity = arg_traits::find_verbosity_flag(argn, argv);
  bool is_recursive = arg_traits::find_recursive_flag(argn, argv);

  std::vector<std::string> exception_flags = arg_traits::find_exception_contents(argn, argv);
   
  file::file_type context_type  = file::get_file_type(context);

  if(old_word == new_word)
  {
    std::cout << "the words are the same!" << std::endl;
      std::exit(1);
  }

  if(context_type == file::file_type::Unknown)
  {
    std::cout << "error: context file '" << context << "' " << "is unknown or not exists." << std::endl;
    std::exit(1);
  }

  /* pipeline to files */
  if(context_type == file::file_type::Regular) 
  {
    if(is_recursive && is_verbosity)
    {
      std::cout << "ignoring the '-r' flag." << std::endl;
    }
    if(std::filesystem::file_size(context) <= MB10) file::ligth_pipeline(old_word, new_word, context, is_verbosity);
    else file::heavy_pipeline(old_word, new_word, context, is_verbosity); 
    std::exit(0);
  }
  
  /* --- */
  /* pipieline for directories */
  if(context_type == file::file_type::Directory)
  {
    if(is_recursive)
    {
      if(is_verbosity)
      {
        std::cout << "starting recursive in directory: " << std::filesystem::absolute(context) << std::endl;
      }
      file::dir_recusive_pipeline(context, old_word, new_word, exception_flags, is_verbosity);
      if(is_verbosity)
        std::cout << "done." << std::endl;
      std::exit(0);
    }
      file::dir_pipeline(context, old_word, new_word, exception_flags, is_verbosity);
      std::exit(0);
  }
}
