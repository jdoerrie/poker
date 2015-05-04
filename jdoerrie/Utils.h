#pragma once

#include <string>

enum class Category;
enum class GameType;
enum class Rank;
enum class Suit;

class Equity;

class Utils {
 public:
  static Rank toRank(char c);
  static Suit toSuit(char c);

  static char toChar(Rank rank);
  static char toChar(Suit suit);

  static std::string toString(Category category);

  static std::string getBashColor(Suit suit);
  static std::string colorEquity(const Equity& equity);

  static int getNumCards(GameType gameType);
  static std::string getGameTypeName(GameType gameType);
  static GameType getGameType(const std::string& name);
};
