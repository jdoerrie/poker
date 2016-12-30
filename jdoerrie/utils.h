#include "suit.h"

constexpr const char* GetBashColor(Suit suit) {
  switch (suit) {
    case Suit::CLUBS:
      return "\x1B[0;32m";  // Green
    case Suit::DIAMONDS:
      return "\x1B[0;34m";  // Blue
    case Suit::HEARTS:
      return "\x1B[0;31m";  // Red
    case Suit::SPADES:
      return "\x1B[0;37m";  // White
    default:
      return "\x1B[0m";  // Blank
  };
}
