#pragma once

#include <vector>
#include <string>

enum class GameType;

class CardCollection;

class Range {
 public:
  Range(const std::string& description = "");
  Range(const std::vector<CardCollection>& hands);

  size_t fromRegEx(const std::string& str, GameType gameType);
  const std::vector<CardCollection>& getHands() const;
  Range filter(const CardCollection& hand) const;

 private:
  std::string replaceSuitKeywords(std::string str);
  std::string replaceIntervals(std::string str);
  void normalize();

  std::vector<CardCollection> hands_;
  std::string description_;
  GameType gameType_;
};
