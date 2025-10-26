#ifndef _RRREPLACER_H_
#define _RRREPLACER_H_

#include <string>

namespace replacer
{
  void heavy_pipeline(const std::string& old_word, 
                      const std::string& new_word, 
                      const std::string& file, 
                      bool verbosity = false);
  
  void ligth_pipeline(const std::string& old_word, 
                      const std::string& new_word, 
                      const std::string& file, 
                      bool verbosity = false);
}

#endif
