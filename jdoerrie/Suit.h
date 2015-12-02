#pragma once

#include <string>

enum class Suit {
  INVALID = 0,
  CLUBS = 1,
  DIAMONDS = 2,
  HEARTS = 3,
  SPADES = 4,
};

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
  const char suits[] = "-cdhs";
  return suits[static_cast<size_t>(suit)];
}


constexpr const char* getBashColor(Suit suit) {
  const char* bashColors[] = {
      "\x1B[0m",
      "\x1B[0;32m",  // Clubs    -> Green
      "\x1B[0;34m",  // Diamonds -> Blue
      "\x1B[0;31m",  // Hearts   -> Red
      "\x1B[0;37m"   // Spades   -> White
  };

  return bashColors[static_cast<size_t>(suit)];
}
