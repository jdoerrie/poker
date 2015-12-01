#pragma once

#include <string>

enum class Suit {
  INVALID = 0,
  CLUBS = 1,
  DIAMONDS = 2,
  HEARTS = 3,
  SPADES = 4,
};

Suit toSuit(char c);

char toChar(Suit suit);

std::string getBashColor(Suit suit);
