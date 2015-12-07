#pragma once

#include <vector>

namespace poker {
namespace suit {
enum class Suit {
  INVALID = 0,
  CLUBS = 1,
  DIAMONDS = 2,
  HEARTS = 3,
  SPADES = 4,
};

constexpr Suit fromChar(char c) {
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

template <class Container = std::vector<Suit>>
constexpr Container getAllSuits() {
  Container c;
  for (int i = int(Suit::CLUBS); i <= int(Suit::SPADES); ++i) {
    c.insert(c.end(), Suit(i));
  }

  return c;
}

constexpr size_t getHash(Suit suit) {
  return size_t(0xFFFF) << (16 * (size_t(suit) - 1));
}

constexpr bool isValid(Suit suit) {
  return suit != Suit::INVALID;
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
}  // namespace suit
}  // namespace poker
