#pragma once

#include <fstream>
#include <iostream>
#include <string>

namespace Util {

static std::string loadFileAsString(const std::string &filepath) {
  std::ifstream t(filepath);
  t.seekg(0, std::ios::end);
  size_t size = t.tellg();
  std::string buffer(size, ' ');
  t.seekg(0);
  t.read(&buffer[0], size);
  return buffer;
}

} // namespace Util
