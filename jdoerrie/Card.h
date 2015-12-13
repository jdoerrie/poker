#pragma once

#include "Conversions.h"
#include "Rank.h"
#include "Suit.h"

#include <string>

namespace poker {

class Card {
 public:
  constexpr Card(Rank rank = Rank::INVALID, Suit suit = Suit::INVALID)
      : rank_(rank), suit_(suit) {}

  constexpr Card(char cRank, char cSuit)
      : rank_(toRank(cRank)), suit_(toSuit(cSuit)) {}

  constexpr Card(const std::string& str) : Card(str.at(0), str.at(1)) {}

  constexpr Rank getRank() const { return rank_; }
  constexpr Suit getSuit() const { return suit_; }

 private:
  Rank rank_;
  Suit suit_;
};

}  // poker
