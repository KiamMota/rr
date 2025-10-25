#include "file.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <system_error>
#include <filesystem>
#include <unordered_set>
#include <vector>

file::file_type file::get_file_type(std::string &file_name)
{
    std::error_code ec;
    std::filesystem::file_status status = std::filesystem::symlink_status(file_name, ec);
    if (ec) return file_type::Unknown;
    if (std::filesystem::is_regular_file(status)) return file_type::Regular;
    if (std::filesystem::is_directory(status)) return file_type::Directory;
    if (std::filesystem::is_symlink(status)) return file_type::Symlink;
    return file_type::Unknown;
}

void file::ligth_pipeline(const std::string &old_word, const std::string &new_word, const std::string &file, bool verbosity /* = false */)  
{

  std::ifstream infile(file);
  if (!infile) {
    std::cerr << "Couldn't open file '" << file << "'." << std::endl;
    return;
  }

  std::ofstream outfile(file + ".tmp");
  if (!outfile) {
    std::cerr << "Couldn't create temp file." << std::endl;
    return;
  }

  size_t pos = 0;
  std::string mem_text((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
  while ((pos = mem_text.find(old_word, pos)) != std::string::npos) 
  {
    mem_text.replace(pos, old_word.size(), new_word);
    if (verbosity)
      std::cout << "Replacing '" << old_word << "' with '" << new_word << "' at " << pos << std::endl;
    pos += new_word.size();
  }
  outfile << mem_text;
}

void file::heavy_pipeline(const std::string &old_word, const std::string &new_word, const std::string &file, bool verbosity)
{
  std::ifstream infile(file);
  if (!infile) {
    std::cerr << "Couldn't open file '" << file << "'." << std::endl;
    return;
  }

  std::ofstream outfile(file + ".tmp");
  if (!outfile) {
    std::cerr << "Couldn't create temp file." << std::endl;
    return;
  }

  std::string line;
  while (std::getline(infile, line)) 
  {
    size_t pos = 0;
    while ((pos = line.find(old_word, pos)) != std::string::npos) 
    {
      line.replace(pos, old_word.size(), new_word);
      if (verbosity)
      {
        std::cout << "Replacing '" << old_word << "' with '" << new_word << "' at " << pos << std::endl;
      }
      pos += new_word.size();
    }
    outfile << line << '\n';
    outfile.flush();
  }

    infile.close();
    outfile.close();

    if (std::remove(file.c_str()) != 0) {
        std::cerr << "Error deleting original file." << std::endl;
        return;
    }
    if (std::rename((file + ".tmp").c_str(), file.c_str()) != 0) {
        std::cerr << "Error renaming temp file." << std::endl;
    }
    if(verbosity) std::cout << "done!" << std::endl;
}

void file::dir_pipeline(const std::string &old_word, const std::string &new_word, const std::string &dir_name, const std::vector<std::string> exceptions, bool verbosity)
{
  std::filesystem::path path = dir_name;
  std::unordered_set<std::string> exc_set(exceptions.begin(), exceptions.end());

  for (const auto& entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      if (verbosity)
        std::cout << "replacing in: " << entry.path() << std::endl;

      if (exc_set.find(entry.path().string()) == exc_set.end())
      {
        if(entry.file_size() <= MB10)
        {
          ligth_pipeline(old_word, new_word, entry.path().filename().string(), verbosity);
        }else{ 
          heavy_pipeline(old_word, new_word, entry.path().filename().string(), verbosity);
        }   
      }
    }
  }
}

void file::dir_recusive_pipeline(const std::string &old_word, const std::string &new_word, const std::string &dir_name, const std::vector<std::string> exceptions, bool verbosity)
{
 
  std::filesystem::path path = dir_name;
  std::vector<std::filesystem::path> files;

  for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
    if (entry.is_regular_file())
      files.push_back(entry.path());
  }

  for (const auto& file : files) {
    if (verbosity)
      std::cout << "replacing in: " << file << '\n';

    if (std::find(exceptions.begin(), exceptions.end(), file.string()) == exceptions.end())
    {
      if(std::filesystem::file_size(file) <= MB10)
      {
        ligth_pipeline(old_word, new_word, file.string(), verbosity);
      }
      else
      {
        heavy_pipeline(old_word, new_word, file.string(), verbosity);
      }
    }
  }
}


