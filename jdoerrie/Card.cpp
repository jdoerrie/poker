#include "Card.h"
#include "Utils.h"
#include <string>

using namespace std;

const size_t Card::MAX_ID = 52;

Card::Card(Rank rank, Suit suit) : rank_(rank), suit_(suit) {}

Card::Card(char cRank, char cSuit) {
  rank_ = Utils::toRank(cRank);
  suit_ = Utils::toSuit(cSuit);
}

Card::Card(const string& str) {
  rank_ = Rank::INVALID;
  suit_ = Suit::INVALID;

  if (str.size() != 2) {
    return;
  }

  rank_ = Utils::toRank(str[0]);
  suit_ = Utils::toSuit(str[1]);
}

Card::Card(size_t id) {
  --id;
  rank_ = static_cast<Rank>(id / 4 + 2);
  suit_ = static_cast<Suit>(id % 4 + 1);
}

Rank Card::getRank() const {
  return rank_;
}

Suit Card::getSuit() const {
  return suit_;
}

size_t Card::getId() const {
  if (!isValid()) {
    return 0;
  }

  size_t cardId = (static_cast<size_t>(rank_) - 2) * 4
                + (static_cast<size_t>(suit_) - 1) + 1;

  return cardId;
}

bool Card::isValid() const {
  return rank_ != Rank::INVALID && suit_ != Suit::INVALID;
}

vector<Card> Card::enumerateAllCards() {
  vector<Card> cards;
  for (int id = 0; id < MAX_ID; ++id) {
    cards.emplace_back(id + 1);
  }

  return cards;
}

string Card::toString() const {
  return {Utils::toChar(rank_), Utils::toChar(suit_)};
}

bool Card::operator<(const Card& other) const {
  return getId() < other.getId();
}

bool Card::operator==(const Card& other) const {
  return getId() == other.getId();
}

ostream& operator<<(ostream& out, const Card& c) {
  out << c.toString();
  return out;
}

template<>
struct hash<Card> {
  size_t operator()(const Card& c) const {
    return c.getId();
  }
};
