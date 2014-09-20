#pragma once

#include "GameType.h"
#include "Rank.h"
#include "Suit.h"
#include <string>

class Utils {
 public:
  static Rank toRank(char c);
  static Suit toSuit(char c);

  static char toChar(Rank rank);
  static char toChar(Suit suit);

  static int getNumCards(GameType gameType);
  static std::string getGameTypeName(GameType gameType);
  static GameType getGameType(const std::string& name);
};
