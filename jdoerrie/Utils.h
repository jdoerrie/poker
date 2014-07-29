#pragma once
#include <vector>
#include <string>

class Utils {
 public:
  static std::vector<std::string> tokenize(const std::string& str, char delim) {
    std::vector<std::string> result;
    size_t prev = 0, curr;
    curr = str.find(delim);
    while (curr != std::string::npos) {
      result.push_back(str.substr(prev, curr));
      prev = curr + 1;
      curr = str.find(delim, prev);
    }

    result.push_back(str.substr(prev));
    return result;
  }
};
