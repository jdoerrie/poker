#include "game_type.h"

int getNumCards(GameType gameType) {
  switch (gameType) {
    case GameType::HOLDEM:
      return 2;

    case GameType::OMAHA:
      return 4;

    case GameType::OMAHA5:
      return 5;

    default:
      return 0;
  }
}

std::string getGameTypeName(GameType gameType) {
  switch (gameType) {
    case GameType::HOLDEM:
      return "holdem";

    case GameType::OMAHA:
      return "omaha";

    case GameType::OMAHA5:
      return "omaha5";

    default:
      return "";
  }
}

GameType getGameType(const std::string& name) {
  if (name == "holdem") {
    return GameType::HOLDEM;
  } else if (name == "omaha") {
    return GameType::OMAHA;
  } else if (name == "omaha5") {
    return GameType::OMAHA5;
  } else {
    return GameType::NONE;
  }
}
