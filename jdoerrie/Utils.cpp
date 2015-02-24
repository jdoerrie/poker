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
  switch(rank) {
    case Rank::TWO:
      return '2';

    case Rank::THREE:
      return '3';

    case Rank::FOUR:
      return '4';

    case Rank::FIVE:
      return '5';

    case Rank::SIX:
      return '6';

    case Rank::SEVEN:
      return '7';

    case Rank::EIGHT:
      return '8';

    case Rank::NINE:
      return '9';

    case Rank::TEN:
      return 'T';

    case Rank::JACK:
      return 'J';

    case Rank::QUEEN:
      return 'Q';

    case Rank::KING:
      return 'K';

    case Rank::ACE:
      return 'A';

    default:
      return '-';
  }
}

char Utils::toChar(Suit suit) {
  switch(suit) {
    case Suit::CLUBS:
      return 'c';

    case Suit::DIAMONDS:
      return 'd';

    case Suit::HEARTS:
      return 'h';

    case Suit::SPADES:
      return 's';

    default:
      return '-';
  }
}

string Utils::getBashColor(Suit suit) {
  switch(suit) {
    case Suit::CLUBS:
      return "\e[1;32m";

    case Suit::DIAMONDS:
      return "\e[1;34m";

    case Suit::HEARTS:
      return "\e[1;31m";

    case Suit::SPADES:
      return "\e[1;37m";

    default:
      return "\e[0m";
  }
}

string Utils::colorEquity(const Equity& equity) {
  static vector<string> gradient = {
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
