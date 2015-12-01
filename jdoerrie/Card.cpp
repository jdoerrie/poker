#include "Card.h"

#include <string>

using namespace std;

const size_t Card::MAX_ID = 52;
const size_t Card::INVALID_ID = 0;

Card::Card(Rank rank, Suit suit) : rank_(rank), suit_(suit) {
  id_ = isValid() ? computeId() : INVALID_ID;
}

Card::Card(char cRank, char cSuit) {
  rank_ = toRank(cRank);
  suit_ = toSuit(cSuit);
  id_ = isValid() ? computeId() : INVALID_ID;
}

Card::Card(const string& str)
    : rank_(Rank::INVALID), suit_(Suit::INVALID), id_(INVALID_ID) {
  if (str.size() != 2) {
    return;
  }

  rank_ = toRank(str[0]);
  suit_ = toSuit(str[1]);
  id_ = isValid() ? computeId() : INVALID_ID;
}

Card::Card(size_t id) : id_(id) {
  --id;
  rank_ = static_cast<Rank>(id / 4 + 2);
  suit_ = static_cast<Suit>(id % 4 + 1);
  id_ = isValid() ? id_ : INVALID_ID;
}

Rank Card::getRank() const { return rank_; }

Suit Card::getSuit() const { return suit_; }

size_t Card::getId() const { return id_; }

bool Card::isValid() const {
  return rank_ != Rank::INVALID && suit_ != Suit::INVALID;
}

vector<Card> Card::enumerateAllCards() {
  vector<Card> cards;
  for (size_t id = 0; id < MAX_ID; ++id) {
    cards.emplace_back(id + 1);
  }

  return cards;
}

string Card::toString(bool useColors) const {
  if (useColors) {
    return getBashColor(suit_) + toChar(rank_) + toChar(suit_) +
           getBashColor(Suit::INVALID);
  } else {
    return {toChar(rank_), toChar(suit_)};
  }
}

bool Card::operator<(const Card& other) const { return id_ < other.id_; }

bool Card::operator==(const Card& other) const { return id_ == other.id_; }

size_t Card::computeId() const {
  return (static_cast<size_t>(rank_) - 2) * 4 +
         (static_cast<size_t>(suit_) - 1) + 1;
}
