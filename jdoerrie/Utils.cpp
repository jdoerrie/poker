#include "Utils.h"

using namespace std;

Rank Utils::toRank(char c) {
  switch(c) {
    case '2':
      return Rank::TWO;

    case '3':
      return Rank::THREE;

    case '4':
      return Rank::FOUR;

    case '5':
      return Rank::FIVE;

    case '6':
      return Rank::SIX;

    case '7':
      return Rank::SEVEN;

    case '8':
      return Rank::EIGHT;

    case '9':
      return Rank::NINE;

    case 't':
    case 'T':
      return Rank::TEN;

    case 'j':
    case 'J':
      return Rank::JACK;

    case 'q':
    case 'Q':
      return Rank::QUEEN;

    case 'k':
    case 'K':
      return Rank::KING;

    case 'a':
    case 'A':
      return Rank::ACE;

    default:
      return Rank::INVALID;
  }
}

Suit Utils::toSuit(char c) {
  switch(c) {
    case 'c':
    case 'C':
      return Suit::CLUBS;

    case 'd':
    case 'D':
      return Suit::DIAMONDS;

    case 'h':
    case 'H':
      return Suit::HEARTS;

    case 's':
    case 'S':
      return Suit::SPADES;

    default:
      return Suit::INVALID;
  }
}

char Utils::toChar(Rank rank) {
  const char ranks[] = "--23456789TJQKA";
  return ranks[static_cast<size_t>(rank)];
}

char Utils::toChar(Suit suit) {
  const char suits[] = "-cdhs";
  return suits[static_cast<size_t>(suit)];
}

string Utils::toString(Category category) {
  const string categories[] = {
    "Invalid",
    "High Card",
    "Pair",
    "Two Pair",
    "Three of a Kind",
    "Straight",
    "Flush",
    "Full House",
    "Four of a Kind",
    "Straight Flush"
  };

  return categories[static_cast<size_t>(category)];
}


string Utils::getBashColor(Suit suit) {
  const string bashColors[] = {
    "\e[0m",
    "\e[1;32m", // Clubs    -> Bold Green
    "\e[1;34m", // Diamonds -> Bold Blue
    "\e[1;31m", // Hearts   -> Bold Red
    "\e[1;37m"  // Spades   -> Bold White
  };

  return bashColors[static_cast<size_t>(suit)];
}

string Utils::colorEquity(const Equity& equity) {
  const vector<string> gradient = {
    "\e[38;5;196m",
    "\e[38;5;202m",
    "\e[38;5;208m",
    "\e[38;5;214m",
    "\e[38;5;220m",
    "\e[38;5;226m",
    "\e[38;5;190m",
    "\e[38;5;154m",
    "\e[38;5;118m",
    "\e[38;5;82m",
    "\e[38;5;46m"
  };

  size_t idx = static_cast<size_t>(equity.toDouble() * gradient.size());
  if (idx == gradient.size()) {
    --idx;
  }

  return gradient[idx] + to_string(equity.toDouble()) + "\e[0m";
}

int Utils::getNumCards(GameType gameType) {
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

string Utils::getGameTypeName(GameType gameType) {
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
GameType Utils::getGameType(const string& name) {
  if (name == "holdem") {
    return GameType::HOLDEM;
  } else if (name == "omaha") {
    return GameType::OMAHA;
  } else if (name == "omaha5") {
    return GameType::OMAHA5;
  } else {
    return GameType::INVALID;
  }
}
