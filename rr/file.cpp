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

void file::heavy_pipeline(const std::string &old_word, const std::string &new_word,
                              const std::string &file, bool verbosity)
{
    constexpr size_t BUFFER_SIZE = 16 * 1024 * 1024; // 16 MB buffer

    std::ifstream infile(file, std::ios::binary);
    if (!infile) {
        std::cerr << "Couldn't open file '" << file << "'." << std::endl;
        return;
    }

    std::ofstream outfile(file + ".tmp", std::ios::binary);
    if (!outfile) {
        std::cerr << "Couldn't create temp file." << std::endl;
        return;
    }

    std::vector<char> buffer(BUFFER_SIZE);
    std::string leftover; // Para pegar palavras cortadas no final do buffer

    while (infile) {
        infile.read(buffer.data(), buffer.size());
        std::streamsize bytesRead = infile.gcount();
        if (bytesRead <= 0) break;

        std::string chunk = leftover + std::string(buffer.data(), bytesRead);

        size_t pos = 0;
        while ((pos = chunk.find(old_word, pos)) != std::string::npos) {
            chunk.replace(pos, old_word.size(), new_word);
            if (verbosity) {
                std::cout << "Replacing at " << pos << std::endl;
            }
            pos += new_word.size();
        }

        // Guardar a parte final para o próximo buffer caso corte palavras
        size_t tail_size = old_word.size() - 1;
        if (chunk.size() > tail_size) {
            leftover = chunk.substr(chunk.size() - tail_size);
            outfile.write(chunk.data(), chunk.size() - tail_size);
        } else {
            leftover = chunk;
        }
    }

    if (!leftover.empty()) {
        outfile.write(leftover.data(), leftover.size());
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

    if (verbosity) std::cout << "done!" << std::endl;
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
        if(entry.file_size() <= MB10) ligth_pipeline(old_word, new_word, entry.path().filename().string(), verbosity);
        else heavy_pipeline(old_word, new_word, entry.path().filename().string(), verbosity);
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
      if(std::filesystem::file_size(file) <= MB10)ligth_pipeline(old_word, new_word, file.string(), verbosity);
      else heavy_pipeline(old_word, new_word, file.string(), verbosity);
    }
  }
}


