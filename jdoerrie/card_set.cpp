#include "card_set.h"

#include "card.h"
#include "GameType.h"

#include <algorithm>

using namespace std;

CardSet::CardSet(const string& str) : id_(0) {
  if (str.size() % 2 == 1) {
    id_ = INVALID_ID;
    return;
  }

  for (size_t i = 0; i < str.size(); i += 2) {
    cards_.emplace_back(str[i], str[i + 1]);
    id_ |= 1LL << cards_.back().GetId();
  }

  normalize();
}

CardSet::CardSet(const vector<Card>& cards)
    : id_(0), cards_(cards) {
  for (const auto& card : cards_) {
    id_ |= 1LL << card.GetId();
  }

  normalize();
}

CardSet::CardSet(size_t id) : id_(id) {
  while (id != 0) {
    int cardId = __builtin_ctzll(id);
    cards_.emplace_back(cardId);
    id &= ~(1LL << cardId);
  }

  std::reverse(std::begin(cards_), std::end(cards_));
}

const vector<Card>& CardSet::getCards() const { return cards_; }

size_t CardSet::GetId() const { return id_; }

vector<CardSet> CardSet::enumerateAllHands(GameType gameType) {
  vector<CardSet> hands;
  enumerateAllHelper(hands, {}, getNumCards(gameType));
  sort(hands.begin(), hands.end());
  hands.erase(unique(hands.begin(), hands.end()), hands.end());
  return hands;
}

bool CardSet::isValid(size_t numCards) const {
  return !(id_ & 1LL) &&
         __builtin_popcountll(id_) == static_cast<int>(numCards);
}

vector<CardSet> CardSet::enumerateAllBoards(
    const CardSet& initialBoard, const CardSet& deadCards) {
  int numCards = max(0, 5 - static_cast<int>(initialBoard.getCards().size()));
  vector<CardSet> boards;
  enumerateAllHelper(boards, initialBoard, numCards, deadCards);
  sort(std::begin(boards), std::end(boards));
  boards.erase(unique(std::begin(boards), std::end(boards)), std::end(boards));
  return boards;
}

bool CardSet::addCard(const Card& card) {
  if (containsCard(card)) {
    return false;
  }

  id_ |= (1LL << card.GetId());
  auto lower = lower_bound(cards_.rbegin(), cards_.rend(), card);
  cards_.insert(lower.base(), card);
  return true;
}

bool CardSet::containsCard(const Card& card) const {
  return id_ & (1LL << card.GetId());
}

string CardSet::toString(bool allRanksFirst) const {
  if (allRanksFirst) {
    string ranksStr, suitsStr;
    for (const Card& card : cards_) {
      ranksStr += ::rank::ToChar(card.rank());
      suitsStr += ::suit::ToChar(card.suit());
    }

    return ranksStr + suitsStr;
  } else {
    string str;
    for (const Card& card : cards_) {
      str += card.ToString(/* useColor */);
    }

    return str;
  }
}

bool CardSet::operator<(const CardSet& other) const {
  return cards_ < other.cards_;
}

bool CardSet::operator==(const CardSet& other) const {
  return cards_ == other.cards_;
}

void CardSet::normalize() {
  sort(cards_.rbegin(), cards_.rend());
  cards_.erase(unique(cards_.begin(), cards_.end()), cards_.end());
}

void CardSet::enumerateAllHelper(vector<CardSet>& hands,
                                        const CardSet& currHand,
                                        size_t numCards,
                                        const CardSet& deadCards) {
  if (numCards == 0) {
    hands.push_back(currHand);
    return;
  }

  for (const auto& card : card::EnumerateAllCards()) {
    if (currHand.containsCard(card) || deadCards.containsCard(card)) {
      continue;
    }

    CardSet nextHand = currHand;
    nextHand.addCard(card);
    enumerateAllHelper(hands, nextHand, numCards - 1, deadCards);
  }
}
