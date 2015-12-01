#include "Suit.h"

#include <array>
#include <cstdint>

Suit toSuit(char c) {
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
}

char toChar(Suit suit) {
  static const char suits[] = "-cdhs";
  return suits[static_cast<size_t>(suit)];
}

std::string getBashColor(Suit suit) {
  static const std::array<std::string, 5> bashColors = {{
      "\x1B[0m",
      "\x1B[0;32m",  // Clubs    -> Green
      "\x1B[0;34m",  // Diamonds -> Blue
      "\x1B[0;31m",  // Hearts   -> Red
      "\x1B[0;37m"   // Spades   -> White
  }};

  return bashColors[static_cast<size_t>(suit)];
}
