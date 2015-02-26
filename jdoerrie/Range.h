#pragma once

#include "GameType.h"
#include <vector>
#include <string>

class Hand;

class Range {
 public:
  Range(const std::string& description = "");
  Range(const std::vector<Hand>& hands);

  size_t fromRegEx(const std::string& str, GameType gameType);
  const std::vector<Hand>& getHands() const;

 private:
  std::string replaceSuitKeywords(std::string str);
  std::string replaceIntervals(std::string str);
  void normalize();

  std::vector<Hand> hands_;
  std::string description_;
  GameType gameType_;
};
