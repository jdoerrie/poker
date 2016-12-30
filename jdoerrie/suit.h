#pragma once

#include <array>

enum class Suit {
  CLUBS,
  DIAMONDS,
  HEARTS,
  SPADES,
  NONE,
};

namespace suit {

constexpr Suit ToSuit(char c) {
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
    return Suit::NONE;
  }
}

constexpr char ToChar(Suit suit) {
  switch (suit) {
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

constexpr size_t MIN_ID = static_cast<size_t>(Suit::CLUBS);
constexpr size_t MAX_ID = static_cast<size_t>(Suit::SPADES);

constexpr std::array<Suit, MAX_ID + 1> GetAllValidSuits() {
  return {{Suit::CLUBS, Suit::DIAMONDS, Suit::HEARTS, Suit::SPADES}};
}

} // namespace suit
