#pragma once

#include <string>

enum class Suit {
  NONE,
  CLUBS,
  DIAMONDS,
  HEARTS,
  SPADES,
};

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

constexpr size_t SUIT_MIN_ID = static_cast<size_t>(Suit::CLUBS);
constexpr size_t SUIT_MAX_ID = static_cast<size_t>(Suit::SPADES);
