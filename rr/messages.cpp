#include "messages.hpp"
#include <iostream>

void messages::help()
{
    std::cout << "recursive replace usage (-h)" << std::endl;
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

void messages::version()
{
  std::cout << RR_ART_ASCII << std::endl;
  std::cout << "recursive replace version " << RR_VERSION << std::endl;
}
