#include "card.h"

#include <ostream>

Card::Card(Rank rank, Suit suit) : rank_{rank}, suit_(suit) {}

Card::Card(char c_rank, char c_suit)
    : rank_{c_rank}, suit_{c_suit} {}

Card::Card(size_t id)
    : rank_{(id - 1) / Suit::MAX_ID + 1},
      suit_{(id - 1) % Suit::MAX_ID + 1} {}

Card::Card(const std::string& str)
    : rank_{str.size() == 2 ? str[0] : '?'},
      suit_{str.size() == 2 ? str[1] : '?'} {}

Rank Card::rank() const {
  return rank_;
}

Suit Card::suit() const {
  return suit_;
}

size_t Card::id() const {
  return (rank_.id() - 1) * Suit::MAX_ID + suit_.id();
}

bool Card::IsValid() const {
  return rank_.IsValid() && suit_.IsValid();
}

std::string Card::ToString() const {
  return std::string{rank_.ToChar(), suit_.ToChar()};
}

bool operator<(const Card& lhs, const Card& rhs) {
  return lhs.id() < rhs.id();
}

bool operator==(const Card& lhs, const Card& rhs) {
  return lhs.id() == rhs.id();
}

std::ostream& operator<<(std::ostream& out, const Card& c) {
  return out << c.ToString();
}

std::vector<Card> card::EnumerateAllCards() {
  std::vector<Card> cards;
  cards.reserve(Card::MAX_ID);

  auto valid_ranks = rank::GetAllValidRanks();
  auto valid_suits = suit::GetAllValidSuits();

  for (auto rank : valid_ranks) {
    for (auto suit : valid_suits) {
      cards.emplace_back(rank, suit);
    }
  }

  return cards;
}
