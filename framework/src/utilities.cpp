#include "utilities.hpp"

const std::string load_file(const std::string &file_name) {
  std::ifstream infile{file_name};

  if (!infile) {
    throw "File " + file_name + " does not exists";
  }

  return {std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>()};
}
