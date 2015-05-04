#pragma once

#include "Rank.h"
#include "Suit.h"

#include <vector>
#include <string>

class Card {
 public:
  static const size_t MAX_ID;
  static const size_t INVALID_ID;

  Card(Rank rank = Rank::INVALID, Suit suit = Suit::INVALID);
  Card(char cRank, char cSuit);
  Card(const std::string& str);
  Card(size_t id);

  Rank getRank() const;
  Suit getSuit() const;
  size_t getId() const;
  bool isValid() const;
  static std::vector<Card> enumerateAllCards();

  std::string toString(bool useColor = false) const;
  friend std::ostream& operator<<(std::ostream& out, const Card& c) {
    out << c.toString(/* useColor */ true);
    return out;
  }


  bool operator<(const Card& other) const;
  bool operator==(const Card& other) const;

 private:
  size_t computeId() const;

  Rank rank_;
  Suit suit_;
  size_t id_;
};

namespace std {
  template<>
  struct hash<Card> {
    size_t operator()(const Card& c) const {
      return c.getId();
    }
  };
}
