#pragma once

#include <string>

enum class GameType {
  NONE,
  HOLDEM,
  OMAHA,
  OMAHA5,
};

int getNumCards(GameType gameType);
std::string getGameTypeName(GameType gameType);
GameType getGameType(const std::string& name);
