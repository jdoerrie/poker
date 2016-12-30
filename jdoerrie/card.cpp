#include "card.h"

#include <ostream>

#include "utils.h"

Card::Card(Rank rank, Suit suit) : rank_(rank), suit_(suit) {}

Card::Card(char c_rank, char c_suit)
    : rank_(rank::ToRank(c_rank)), suit_(suit::ToSuit(c_suit)) {}

Card::Card(size_t id)
    : rank_(static_cast<Rank>(id / suit::MAX_ID + 1)),
      suit_(static_cast<Suit>(id % suit::MAX_ID + 1)) {}

Card::Card(const std::string& str)
    : rank_(str.size() == 2 ? rank::ToRank(str[0]) : Rank::NONE),
      suit_(str.size() == 2 ? suit::ToSuit(str[1]) : Suit::NONE) {}

Rank Card::rank() const {
  return rank_;
}

Suit Card::suit() const {
  return suit_;
}

size_t Card::GetId() const {
  return (static_cast<size_t>(rank_) - 1) * suit::MAX_ID +
         (static_cast<size_t>(suit_) - 1);
}

bool Card::IsValid() const {
  return rank_ != Rank::NONE && suit_ != Suit::NONE;
}

std::string Card::ToString() const {
  return std::string{rank::ToChar(rank_), suit::ToChar(suit_)};
}

bool operator<(const Card& lhs, const Card& rhs) {
  return lhs.GetId() < rhs.GetId();
}

bool operator==(const Card& lhs, const Card& rhs) {
  return lhs.GetId() == rhs.GetId();
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
