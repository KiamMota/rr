#ifndef _RRFILE_HPP
#define _RRFILE_HPP

#include <vector>
#define MB10 10485760

#include <string>

namespace file
{
  enum class file_type {
    Regular,     // Arquivo normal
    Directory,   // Pasta
    Symlink,     // Link simbÃ³lico
    Character,   // Arquivo de dispositivo de caractere
    Block,       // Arquivo de dispositivo em bloco
    FIFO,        // Named pipe
    Socket,      // Socket
    Unknown      // Tipo desconhecido / nÃ£o suportado
  };

  file::file_type get_file_type(std::string& file_name);
  void dir_pipeline(const std::string& old_word, const std::string& new_word, const std::string& dir_name, const std::vector<std::string> exceptions, bool verbosity = false);
  void dir_recusive_pipeline(const std::string& old_word, const std::string& new_word, const std::string& dir_name, const std::vector<std::string> exceptions, bool verbosity = false);
}

#endif
