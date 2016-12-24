#pragma once

#include "rank.h"
#include "suit.h"

#include <string>
#include <vector>

class Card {
public:
  static constexpr size_t MAX_ID = RANK_MAX_ID * SUIT_MAX_ID;
  static const size_t INVALID_ID = 0;

  Card(Rank rank = Rank::NONE, Suit suit = Suit::NONE);
  Card(char cRank, char cSuit);
  Card(const std::string &str);
  Card(size_t id);

  Rank getRank() const;
  Suit getSuit() const;
  size_t getId() const;
  bool isValid() const;
  static std::vector<Card> enumerateAllCards();

  std::string toString(bool useColor = false) const;
  friend std::ostream &operator<<(std::ostream &out, const Card &c) {
    out << c.toString(/* useColor */ true);
    return out;
  }

  bool operator<(const Card &other) const;
  bool operator==(const Card &other) const;

private:
  size_t computeId() const;

  Rank rank_;
  Suit suit_;
  size_t id_;
};

namespace std {
template <> struct hash<Card> {
  size_t operator()(const Card &c) const { return c.getId(); }
};
}
