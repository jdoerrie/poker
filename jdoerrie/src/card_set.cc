#include "card_set.h"

#include <algorithm>

CardSet::CardSet(const std::bitset<N_BITS>& bits) : bits_{bits} {}

CardSet::CardSet(const Card& card) {
  bits_.set(card.id());
}

CardSet::CardSet(const std::string& str) {
  if (str.size() % 2 == 1) {
    bits_ = INVALID_ID;
  }

  for (auto i = size_t{0}; i < str.size(); i += 2) {
    bits_.set(Card{str[i], str[i + 1]}.id());
  }
}

CardSet::CardSet(const std::vector<Card>& cards) {
  for (const auto& card : cards) {
    bits_.set(card.id());
  }
}

CardSet::CardSet(size_t id) : bits_{id} {}

bool CardSet::IsValid(size_t num_cards) const {
  return !bits_.test(0) && size() == num_cards;
}

bool CardSet::Add(const Card& card) {
  if (Contains(card)) {
    return false;
  }

  bits_.set(card.id());
  return true;
}

CardSet& CardSet::Add(const CardSet& other) {
  return *this |= other;
}

CardSet& CardSet::Remove(const Card& card) {
  bits_.reset(card.id());
  return *this;
}

CardSet& CardSet::Remove(const CardSet& other) {
  return *this -= other;
}

bool CardSet::Contains(const Card& card) const {
  return bits_.test(card.id());
}

bool CardSet::Contains(const CardSet& other) const {
  for (size_t i = 0; i < N_BITS; ++i) {
    if (other.bits_[i] && !bits_[i]) {
      return false;
    }
  }

  return true;
}

CardSet& CardSet::operator&=(const CardSet& other) {
  bits_ &= other.bits_;
  return *this;
}

CardSet& CardSet::operator|=(const CardSet& other) {
  bits_ |= other.bits_;
  return *this;
}

CardSet& CardSet::operator^=(const CardSet& other) {
  bits_ ^= other.bits_;
  return *this;
}

CardSet& CardSet::operator-=(const CardSet& other) {
  for (size_t i = 0; i < N_BITS; ++i) {
    if (other.bits_[i]) {
      bits_[i] = false;
    }
  }

  return *this;
}

CardSet CardSet::operator~() const {
  return CardSet(~bits_);
}

std::vector<int> CardSet::ToIds() const {
  std::vector<int> ids;
  ids.reserve(size());
  for (int i = bits_.size() - 1; i >= 0; --i) {
    if (bits_[i]) {
      ids.emplace_back(i);
    }
  }

  return ids;
}

std::vector<Card> CardSet::ToCards() const {
  std::vector<Card> cards;
  cards.reserve(size());
  for (int i = bits_.size() - 1; i >= 0; --i) {
    if (bits_.test(i)) {
      cards.emplace_back(i);
    }
  }

  return cards;
}

std::string CardSet::ToString(bool allRanksFirst) const {
  if (allRanksFirst) {
    std::string ranksStr, suitsStr;
    for (const Card& card : ToCards()) {
      ranksStr += card.rank().ToChar();
      suitsStr += card.suit().ToChar();
    }

    return ranksStr + suitsStr;
  } else {
    std::string str;
    for (const Card& card : ToCards()) {
      str += card.ToString();
    }

    return str;
  }
}

CardSet operator&(const CardSet& lhs, const CardSet& rhs) {
  return CardSet(lhs) &= rhs;
}

CardSet operator|(const CardSet& lhs, const CardSet& rhs) {
  return CardSet(lhs) |= rhs;
}

CardSet operator^(const CardSet& lhs, const CardSet& rhs) {
  return CardSet(lhs) ^= rhs;
}

CardSet operator-(const CardSet& lhs, const CardSet& rhs) {
  return CardSet(lhs) -= rhs;
}

bool operator<(const CardSet& lhs, const CardSet& rhs) {
  return lhs.id() < rhs.id();
}

bool operator==(const CardSet& lhs, const CardSet& rhs) {
  return lhs.id() == rhs.id();
}

bool operator<=(const CardSet& lhs, const CardSet& rhs) {
  return rhs.Contains(lhs);
}

bool operator>=(const CardSet& lhs, const CardSet& rhs) {
  return lhs.Contains(rhs);
}

std::ostream& operator<<(std::ostream& out, const CardSet& hand) {
  out << hand.ToString();
  return out;
}

void EnumerateAllHelper(std::vector<CardSet>& hands,
                        const CardSet& currHand,
                        size_t numCards,
                        const CardSet& deadCards) {
  if (numCards == 0) {
    hands.push_back(currHand);
    return;
  }

  for (const auto& card : card::EnumerateAllCards()) {
    if (currHand.Contains(card) || deadCards.Contains(card)) {
      continue;
    }

    CardSet nextHand = currHand;
    nextHand.Add(card);
    EnumerateAllHelper(hands, nextHand, numCards - 1, deadCards);
  }
}

std::vector<CardSet> EnumerateAllHands(GameType gameType,
                                       const CardSet& deadCards) {
  std::vector<CardSet> hands;
  EnumerateAllHelper(hands, {}, getNumCards(gameType), deadCards);
  sort(hands.begin(), hands.end());
  hands.erase(unique(hands.begin(), hands.end()), hands.end());
  return hands;
}

std::vector<CardSet> EnumerateAllBoards(const CardSet& initialBoard,
                                        const CardSet& deadCards) {
  int numCards = std::max(0, 5 - static_cast<int>(initialBoard.size()));
  std::vector<CardSet> boards;
  EnumerateAllHelper(boards, initialBoard, numCards, deadCards);
  sort(std::begin(boards), std::end(boards));
  boards.erase(unique(std::begin(boards), std::end(boards)), std::end(boards));
  return boards;
}

void printFormatted(const std::vector<CardSet>& cards,
                    std::ostream& out,
                    const std::string& padding,
                    size_t lineLength,
                    const std::string& seperator) {
  out << padding;
  size_t currLength = padding.size();
  for (const CardSet& hand : cards) {
    if (currLength + hand.ToString().size() + seperator.size() > lineLength) {
      out << std::endl << padding;
      currLength = padding.size();
    }

    out << hand << seperator;
    currLength += hand.ToString().size() + seperator.size();
  }

  out << std::endl;
}
