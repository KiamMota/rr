#include "pipeline.hpp"
#include <fstream>
#include <iostream>
#include <vector>

void replacer::ligth_pipeline(const std::string &old_word, const std::string &new_word, const std::string &file, bool verbosity /* = false */)  
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

void replacer::heavy_pipeline(const std::string &old_word, const std::string &new_word,
                              const std::string &file, bool verbosity)
{
    constexpr size_t BUFFER_SIZE = (16 * 1024 * 1024); 

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
    
    std::string leftover; 

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
