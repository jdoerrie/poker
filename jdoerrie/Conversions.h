#pragma once

#include "GameType.h"
#include "Rank.h"
#include "Suit.h"

#include <cstdint>
#include <string>

namespace poker {

constexpr Rank toRank(char c) {
  switch (c) {
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

constexpr char toChar(Rank rank) {
  constexpr char ranks[] = "--23456789TJQKA";
  return ranks[size_t(rank)];
}

constexpr Suit toSuit(char c) {
  switch (c) {
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
};

constexpr char toChar(Suit suit) {
  constexpr char suits[] = "-cdhs";
  return suits[size_t(suit)];
}

constexpr size_t getNumCards(GameType gameType) {
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
    return GameType::INVALID;
  }
}

}  // poker
