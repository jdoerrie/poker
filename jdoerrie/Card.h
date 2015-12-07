#pragma once

#include "Rank.h"
#include "Suit.h"

#include <vector>
#include <string>
#include <cassert>

namespace poker {
namespace card {

using poker::rank::Rank;
using poker::suit::Suit;

class Card {
 public:
  constexpr Card(Rank rank = Rank::INVALID, Suit suit = Suit::INVALID)
      : rank_(rank), suit_(suit) {}

  constexpr Card(char cRank, char cSuit)
      : rank_(rank::fromChar(cRank)), suit_(suit::fromChar(cSuit)) {}

  constexpr Card(const std::string& str): Card(str.at(0), str.at(1)) {}

  constexpr Rank getRank() const { return rank_; }
  constexpr Suit getSuit() const { return suit_; }

  std::string toString(bool useColor = false) const;
  friend std::ostream& operator<<(std::ostream& out, const Card& c) {
    out << c.toString(/* useColor */ true);
    return out;
  }

  bool operator<(const Card& other) const;
  bool operator==(const Card& other) const;

 private:
  const Rank rank_;
  const Suit suit_;
};

constexpr size_t getHash(Card card) {
  return rank::getHash(card.getRank()) & suit::getHash(card.getSuit());
}

constexpr bool isValid(Card card) {
  return rank::isValid(card.getRank()) && suit::isValid(card.getSuit());
}

template <class Container = std::vector<Card>>
Container getAllCards() {
  Container c;
  for (auto&& rank : rank::getAllRanks()) {
    for (auto&& suit : suit::getAllSuits()) {
      c.emplace(c.end(), rank, suit);
    }
  }

  return c;
}
}  // namespace card
}  // namespace poker

namespace std {
using namespace poker::card;
template <>
struct hash<Card> {
  size_t operator()(const Card& card) const { return getHash(card); }
};
}
