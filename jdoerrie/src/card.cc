#include "card.h"

Card::Card(const std::string& str)
    : rank_{str.size() == 2 ? to_rank(str[0]) : Rank::NONE},
      suit_{str.size() == 2 ? str[1] : '?'} {}

bool operator==(const Card& lhs, const Card& rhs) {
  return lhs.id() == rhs.id();
}

bool operator!=(const Card& lhs, const Card& rhs) {
  return !(lhs == rhs);
}

bool operator<(const Card& lhs, const Card& rhs) {
  return lhs.id() < rhs.id();
}

bool operator<=(const Card& lhs, const Card& rhs) {
  return !(rhs < lhs);
}

bool operator>(const Card& lhs, const Card& rhs) {
  return rhs < lhs;
}

bool operator>=(const Card& lhs, const Card& rhs) {
  return !(lhs < rhs);
}

std::ostream& operator<<(std::ostream& out, const Card& c) {
  return out << std::string(c);
}

std::vector<Card> card::EnumerateAllCards() {
  std::vector<Card> cards;
  cards.reserve(Card::MAX_ID);

  for (auto rank : rank::all_ranks()) {
    for (auto suit : suit::GetAllValidSuits()) {
      cards.emplace_back(rank, suit);
    }
  }

  return cards;
}
