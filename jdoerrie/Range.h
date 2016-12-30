#pragma once

#include <string>
#include <vector>

enum class GameType;

class CardSet;

class Range {
 public:
  Range(const std::string& description = "");
  Range(const std::vector<CardSet>& hands);

  size_t fromRegEx(const std::string& str, GameType gameType);
  const std::vector<CardSet>& getHands() const;
  Range filter(const CardSet& hand) const;

 private:
  std::string replaceSuitKeywords(std::string str);
  std::string replaceIntervals(std::string str);
  void normalize();

  std::vector<CardSet> hands_;
  std::string description_;
  GameType gameType_;
};
