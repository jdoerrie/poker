#include "card_collection.h"

#include "Card.h"
#include "GameType.h"

#include <algorithm>

using namespace std;

const size_t CardCollection::MAX_ID = 0x1ffffffffffffeLL;
const size_t CardCollection::INVALID_ID = 1;

CardCollection::CardCollection(const string& str) : id_(INVALID_ID) {
  if (str.size() % 2 == 1) {
    return;
  }

  id_ = 0;
  for (size_t i = 0; i < str.size(); i += 2) {
    cards_.emplace_back(str[i], str[i + 1]);
    id_ |= 1LL << cards_.back().getId();
  }

  normalize();
}

CardCollection::CardCollection(const vector<Card>& cards) : id_(INVALID_ID), cards_(cards) {
  id_ = 0;
  for (const auto& card : cards) {
    id_ |= 1LL << card.getId();
  }

  normalize();
}

CardCollection::CardCollection(size_t id) : id_(id) {
  while (id != 0) {
    int cardId = __builtin_ctzll(id);
    cards_.emplace_back(cardId);
    id &= ~(1LL << cardId);
  }

  std::reverse(std::begin(cards_), std::end(cards_));
}

const vector<Card>& CardCollection::getCards() const { return cards_; }

size_t CardCollection::getId() const { return id_; }

vector<CardCollection> CardCollection::enumerateAllHands(GameType gameType) {
  vector<CardCollection> hands;
  enumerateAllHelper(hands, {}, getNumCards(gameType));
  sort(hands.begin(), hands.end());
  hands.erase(unique(hands.begin(), hands.end()), hands.end());
  return hands;
}

bool CardCollection::isValid(size_t numCards) const {
  return !(id_ & 1LL) &&
         __builtin_popcountll(id_) == static_cast<int>(numCards);
}

vector<CardCollection> CardCollection::enumerateAllBoards(const CardCollection& initialBoard,
                                      const CardCollection& deadCards) {
  int numCards = max(0, 5 - static_cast<int>(initialBoard.getCards().size()));
  vector<CardCollection> boards;
  enumerateAllHelper(boards, initialBoard, numCards, deadCards);
  sort(std::begin(boards), std::end(boards));
  boards.erase(unique(std::begin(boards), std::end(boards)), std::end(boards));
  return boards;
}

bool CardCollection::addCard(const Card& card) {
  if (containsCard(card)) {
    return false;
  }

  id_ |= (1LL << card.getId());
  auto lower = lower_bound(cards_.rbegin(), cards_.rend(), card);
  cards_.insert(lower.base(), card);
  return true;
}

bool CardCollection::containsCard(const Card& card) const {
  return id_ & (1LL << card.getId());
}

string CardCollection::toString(bool allRanksFirst, bool useColor) const {
  if (allRanksFirst) {
    string ranksStr, suitsStr;
    for (const Card& card : cards_) {
      ranksStr += toChar(card.getRank());
      suitsStr += toChar(card.getSuit());
    }

    return ranksStr + suitsStr;
  } else {
    string str;
    for (const Card& card : cards_) {
      str += card.toString(useColor);
    }

    return str;
  }
}

bool CardCollection::operator<(const CardCollection& other) const { return cards_ < other.cards_; }

bool CardCollection::operator==(const CardCollection& other) const {
  return cards_ == other.cards_;
}

void CardCollection::normalize() {
  sort(cards_.rbegin(), cards_.rend());
  cards_.erase(unique(cards_.begin(), cards_.end()), cards_.end());
}

void CardCollection::enumerateAllHelper(vector<CardCollection>& hands, const CardCollection& currHand,
                              size_t numCards, const CardCollection& deadCards) {
  if (numCards == 0) {
    hands.push_back(currHand);
    return;
  }

  for (size_t nextId = 1; nextId <= Card::MAX_ID; ++nextId) {
    Card nextCard(nextId);
    if (currHand.containsCard(nextCard) || deadCards.containsCard(nextCard)) {
      continue;
    }

    CardCollection nextHand = currHand;
    nextHand.addCard(nextCard);
    enumerateAllHelper(hands, nextHand, numCards - 1);
  }
}
