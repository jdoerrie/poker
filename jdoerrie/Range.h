#pragma once

#include <vector>
#include <string>

enum class GameType;

class Hand;

class Range {
 public:
  Range(const std::string& description = "");
  Range(const std::vector<Hand>& hands);

  size_t fromRegEx(const std::string& str, GameType gameType);
  const std::vector<Hand>& getHands() const;
  Range filter(const Hand& hand) const;

 private:
  std::string replaceSuitKeywords(std::string str);
  std::string replaceIntervals(std::string str);
  void normalize();

  std::vector<Hand> hands_;
  std::string description_;
  GameType gameType_;
};
