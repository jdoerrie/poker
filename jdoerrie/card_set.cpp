#include "card_set.h"

#include "GameType.h"
#include "card.h"

#include <algorithm>

CardSet::CardSet(const std::string& str) {
  if (str.size() % 2 == 1) {
    bits_ = INVALID_ID;
  }

  std::cout << str << std::endl;
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

bool CardSet::empty() const {
  return bits_ == 0;
}

size_t CardSet::size() const {
  return bits_.count();
}

size_t CardSet::id() const {
  return bits_.to_ullong();
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


bool CardSet::IsValid(size_t num_cards) const {
  return !bits_.test(0) && size() == num_cards;
}

bool CardSet::AddCard(const Card& card) {
  if (ContainsCard(card)) {
    return false;
  }

  bits_.set(card.id());
  return true;
}

bool CardSet::ContainsCard(const Card& card) const {
  return bits_.test(card.id());
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

bool operator<(const CardSet& lhs, const CardSet& rhs) {
  return lhs.id() < rhs.id();
}

bool operator==(const CardSet& lhs, const CardSet& rhs) {
  return lhs.id() == rhs.id();
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
    if (currHand.ContainsCard(card) || deadCards.ContainsCard(card)) {
      continue;
    }

    CardSet nextHand = currHand;
    nextHand.AddCard(card);
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
