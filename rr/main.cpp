#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <ostream>
#include <string>
#include <vector>
#include <unordered_set>

#define NOFLAG "__noFLAG"
#define MB10 10485760
#define RR_VERSION "0.3"

namespace fs = std::filesystem;

enum class file_type {
    Regular,     // Arquivo normal
    Directory,   // Pasta
    Symlink,     // Link simbólico
    Character,   // Arquivo de dispositivo de caractere
    Block,       // Arquivo de dispositivo em bloco
    FIFO,        // Named pipe
    Socket,      // Socket
    Unknown      // Tipo desconhecido / não suportado
};

void help()
{
    std::cout << "To use recursive replace:\n";
    std::cout << "  <OLD_WORD> <NEW_WORD> <FILE/DIR/OTHER> |flags|\n\n";
    std::cout << "Flags:\n";

    std::cout << "  -v / --verbosity    : Show each replacement as it happens, with position info.\n";
    std::cout << "                        Useful for debugging or verifying which occurrences are replaced.\n\n";

    std::cout << "  -e / --exceptions   : Skip or handle specific lines or patterns that match user-defined rules.\n";
    std::cout << "                        Can be used to avoid replacing words inside comments, quotes, or other contexts.\n\n";

    std::cout << "  -r / --recursive    : Apply replacements recursively across multiple files or directories.\n";
    std::cout << "                        Useful when you want to process all files in a folder structure automatically.\n\n";

    std::cout << "  -t / --timestamp    : Apply a timestamp or backup to the file before performing replacements.\n";

}

void version()
{
    std::cout << "  _ __ _ __ \n";
    std::cout << " | '__| '__|\n";
    std::cout << " | |  | |   \n";
    std::cout << " |_|  |_|   \n\n";

    std::cout << "recursive replace version " << RR_VERSION << "\r\n";
}


std::vector<std::string> find_excepction_flag(const int& argn, char**& argv)
{
  std::vector<std::string> excepctions;
  for(int i = 0; i<argn; i++)
  {
    if(std::strcmp(argv[i], "-e") == 0 || std::strcmp(argv[i], "--exceptions"))
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

bool find_recursive_flag(const int& argn, char**& argv)
{
  for(int i = 0; i<argn; i++)
  {
    if(std::strcmp(argv[i], "-r") == 0 || std::strcmp(argv[i], "--recursive") == 0)
      return true;
  }
  return false;
}

bool find_time_flag(const int& argn, char**& argv)
{
  for(int i = 0; i<argn; i++)
  {
    if(std::strcmp(argv[i], "-t") == 0 || std::strcmp(argv[i], "--timestamp") == 0)
      return true;
  }
  return false;
}

bool find_verbosity_flag(const int& argn, char**& argv)
{
  for(int i = 0; i<argn; i++)
  {
    if(std::strcmp(argv[i], "-v") == 0)
    {
      std::cout << "starting rr with verbosity." << std::endl;
      return true; 
    }
      
  }
  return false;
}

file_type get_file_type(const std::string& file) {
    std::error_code ec;
    fs::file_status status = fs::symlink_status(file, ec);
    if (ec) return file_type::Unknown;
    if (fs::is_regular_file(status)) return file_type::Regular;
    if (fs::is_directory(status)) return file_type::Directory;
    if (fs::is_symlink(status)) return file_type::Symlink;
    return file_type::Unknown;
}

void pipe_for_file(const std::string& file_name, 
                   const std::string& old_word, 
                   const std::string& new_word, 
                   bool verbosity) 
{
    std::ifstream infile(file_name);
    if (!infile) {
        std::cerr << "Couldn't open file '" << file_name << "'." << std::endl;
        return;
    }

    std::ofstream outfile(file_name + ".tmp");
    if (!outfile) {
        std::cerr << "Couldn't create temp file." << std::endl;
        return;
    }

    if (fs::file_size(file_name) <= MB10) {
        std::string mem_text((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
        size_t pos = 0;
        while ((pos = mem_text.find(old_word, pos)) != std::string::npos) {
            mem_text.replace(pos, old_word.size(), new_word);
            if (verbosity) 
                std::cout << "Replacing '" << old_word << "' with '" << new_word << "' at position " << pos << std::endl;
            pos += new_word.size();
        }
        outfile << mem_text;
    } else {
        std::string line;
        while (std::getline(infile, line)) {
            size_t pos = 0;
            while ((pos = line.find(old_word, pos)) != std::string::npos) {
                line.replace(pos, old_word.size(), new_word);
                if (verbosity) 
                    std::cout << "Replacing '" << old_word << "' with '" << new_word << "' at position " << pos << std::endl;
                pos += new_word.size();
            }
            outfile << line << '\n';
            outfile.flush();
        }
    }

    infile.close();
    outfile.close();

    if (std::remove(file_name.c_str()) != 0) {
        std::cerr << "Error deleting original file." << std::endl;
        return;
    }
    if (std::rename((file_name + ".tmp").c_str(), file_name.c_str()) != 0) {
        std::cerr << "Error renaming temp file." << std::endl;
    }
}

void pipe_for_directory_rec(const std::string& str_path, 
                            const std::string& old_word, 
                            const std::string& new_word,
                            const std::vector<std::string>& exceptions, 
                            bool verbosity)
{
    fs::path path = str_path;
    std::vector<fs::path> files;

    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (entry.is_regular_file())
            files.push_back(entry.path());
    }

    std::sort(files.begin(), files.end()); // ordenação lexicográfica

    for (const auto& file : files) {
        if (verbosity) 
            std::cout << "replacing in: " << file << '\n';

        if (std::find(exceptions.begin(), exceptions.end(), file.string()) == exceptions.end())
            pipe_for_file(file, old_word, new_word, verbosity);
    }
}

void pipe_for_directory(const std::string& str_path, 
                        const std::string& old_word, 
                        const std::string& new_word,
                        const std::vector<std::string>& exceptions,
                        bool verbosity)
{
    fs::path path = str_path;
    std::unordered_set<std::string> exc_set(exceptions.begin(), exceptions.end());

    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            if (verbosity)
                std::cout << "replacing in: " << entry.path() << std::endl;
            
            if (exc_set.find(entry.path().string()) == exc_set.end())
                pipe_for_file(entry.path(), old_word, new_word, verbosity);
        }
    }
}


int main(int argn, char** argv)
{
  /* to skip the command name*/
  argv++;
  /* to sync with the argv */
  argn--;

  if(argn <= 2 && (std::strcmp(argv[0], "-v") == 0 || std::strcmp(argv[0], "--version") == 0))
  {
    version();
    std::exit(0);
  }

  if(argn <= 2)
  {
    help();
    std::exit(0);
  }


  /* parsing arguments */

  bool is_with_timestamp = find_time_flag(argn, argv);
  bool is_recursive = find_recursive_flag(argn, argv);
  bool is_verbosity = find_verbosity_flag(argn, argv);
  std::vector<std::string> exception_flags = find_excepction_flag(argn, argv);

  /* gets the old world */
  std::string old_word = argv[0];
  /* gets the new world */
  std::string new_word = argv[1];
  /* gets the context */
  std::string context = argv[2];
  
  file_type context_type  = get_file_type(context);

  if(context_type == file_type::Unknown)
  {
    std::cout << "context file type is unknown or not exists." << std::endl;
    std::exit(1);
  }

  if(old_word == new_word)
  {
    std::cout << "the words are the same!" << std::endl;
      std::exit(1);
    }

  if(is_verbosity)
    {
    if(!exception_flags.empty())
    {
      std::cout << "files to ignore in exception list: " << std::endl;
      for(const auto& v : exception_flags)
      {
          std::cout << v << std::endl;
      }
    }
  }

  /* pipe to files */
  if(context_type == file_type::Regular) 
  {
    if(is_recursive && is_verbosity)
    {
      std::cout << "starting recursive replace in " << context << " file" << std::endl;
      std::cout << "ignoring the '-r' flag." << std::endl;
    }

    pipe_for_file(context, old_word, new_word, is_verbosity);
    if(is_verbosity)
      std::cout << "done." << std::endl;
    std::exit(1);
  }
  /* pipe to Directory */
  if(context_type == file_type::Directory)
  {
    if(is_recursive)
    {
      if(is_verbosity)
      {
        std::cout << "starting recursive..." << std::endl; 
      }
      pipe_for_directory_rec(context, old_word, new_word, exception_flags, is_verbosity);
      if(is_verbosity)
        std::cout << "done." << std::endl; 
      std::exit(0);
    }
      pipe_for_directory(context, old_word, new_word, exception_flags, is_verbosity);
  }

}


