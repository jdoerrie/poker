#ifndef POKER_CARD_H_
#define POKER_CARD_H_

#include <ostream>
#include <string>
#include <vector>

#include "rank.h"
#include "suit.h"

class Card {
 public:
  static constexpr size_t MAX_ID = MAX_RANK_ID * Suit::MAX_ID;
  static constexpr size_t INVALID_ID = 0;

  constexpr Card(Rank rank, Suit suit) : rank_(rank), suit_(suit) {}
  constexpr Card(char rank, char suit) : rank_(to_rank(rank)), suit_(suit) {}
  constexpr explicit Card(size_t id) : rank_(to_rank(id / Suit::MAX_ID)), suit_(id % Suit::MAX_ID) {}
  explicit Card(const std::string& str);

  constexpr Rank rank() const { return rank_; }
  constexpr Suit suit() const { return suit_; }
  constexpr size_t id() const { return to_id(rank_)  * Suit::MAX_ID + suit_.id(); }
  constexpr bool IsValid() const { return is_valid(rank_) && suit_.IsValid(); }
  std::string ToString() const { return {to_char(rank_), suit_.ToChar()}; }

  explicit operator std::string() const { return ToString(); }

 private:
  Rank rank_;
  Suit suit_;
};

bool operator==(const Card& lhs, const Card& rhs);
bool operator!=(const Card& lhs, const Card& rhs);
bool operator<(const Card& lhs, const Card& rhs);
bool operator<=(const Card& lhs, const Card& rhs);
bool operator>(const Card& lhs, const Card& rhs);
bool operator>=(const Card& lhs, const Card& rhs);

std::ostream& operator<<(std::ostream& out, const Card& c);

namespace card {
std::vector<Card> EnumerateAllCards();
}

#endif  // POKER_CARD_H_
