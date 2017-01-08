#pragma once

#include <iosfwd>
#include <string>
#include <vector>

#include "rank.h"
#include "suit.h"

class Card {
 public:
  static constexpr size_t MAX_ID = Rank::MAX_ID * suit::MAX_ID;
  static constexpr size_t INVALID_ID = 0;

  Card(Rank rank, Suit suit);
  Card(char c_rank, char c_suit);
  explicit Card(size_t id);
  explicit Card(const std::string& str);

  Rank rank() const;
  Suit suit() const;
  size_t id() const;
  bool IsValid() const;
  std::string ToString() const;

 private:
  Rank rank_;
  Suit suit_;
};

bool operator<(const Card& lhs, const Card& rhs);
bool operator==(const Card& lhs, const Card& rhs);
std::ostream& operator<<(std::ostream& out, const Card& c);

namespace card {
std::vector<Card> EnumerateAllCards();
}
