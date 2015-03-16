#include "Hand.h"

#include "Card.h"
#include "Utils.h"
#include <algorithm>

using namespace std;

Hand::Hand(const string& str) {
  if (str.size() % 2 == 1) {
    return;
  }

  for (size_t i = 0; i < str.size(); i += 2) {
    cards_.emplace_back(str[i], str[i+1]);
  }

  normalize();
}

Hand::Hand(vector<Card> cards) {
  cards_ = move(cards);
  normalize();
}

Hand::Hand(size_t id) {
  while (id != 0) {
    cards_.emplace_back(id % (Card::MAX_ID + 1));
    id /= (Card::MAX_ID + 1);
  }

  normalize();
}

const vector<Card>& Hand::getCards() const {
  return cards_;
}

size_t Hand::getId() const {
  size_t id = 0;
  for (const Card& card: cards_) {
    id *= (Card::MAX_ID + 1);
    id += card.getId();
  }

  return id;
}

vector<Hand> Hand::enumerateAllHands(GameType gameType) {
  vector<Hand> hands;
  enumerateAllHelper(hands, {}, Utils::getNumCards(gameType));
  sort(hands.begin(), hands.end());
  hands.erase(unique(hands.begin(), hands.end()), hands.end());
  return hands;
}

bool Hand::isValid(size_t numCards) const {
  if (numCards != 0 && cards_.size() != numCards) {
    return false;
  }

  bool isValid = true;
  for (const Card& card: cards_) {
    isValid &= card.isValid();
  }

  return isValid;
}

vector<Hand> Hand::enumerateAllBoards(
  const Hand& initialBoard,
  const Hand& deadCards) {
    int numCards = max(0, 5 - static_cast<int>(initialBoard.getCards().size()));
  vector<Hand> boards;
  enumerateAllHelper(boards, initialBoard, numCards, deadCards);
  sort(std::begin(boards), std::end(boards));
  boards.erase(unique(std::begin(boards), std::end(boards)), std::end(boards));
  return boards;
}

bool Hand::addCard(const Card& card) {
  auto lower = lower_bound(cards_.rbegin(), cards_.rend(), card);
  if (lower != cards_.rend() && *lower == card) {
    return false;
  }

  cards_.insert(lower.base(), card);
  return true;
}

bool Hand::containsCard(const Card& card) const {
  return binary_search(cards_.rbegin(), cards_.rend(), card);
}

string Hand::toString(bool allRanksFirst, bool useColor) const {
  if (allRanksFirst) {
     string ranksStr, suitsStr;
    for (const Card& card: cards_) {
      ranksStr += Utils::toChar(card.getRank());
      suitsStr += Utils::toChar(card.getSuit());
    }

    return ranksStr + suitsStr;
  } else {
    string str;
    for (const Card& card: cards_) {
      str += card.toString(useColor);
    }

    return str;
  }
}

bool Hand::operator<(const Hand& other) const {
  return cards_ < other.cards_;
}

bool Hand::operator==(const Hand& other) const {
  return cards_ == other.cards_;
}

void Hand::normalize() {
  sort(cards_.rbegin(), cards_.rend());
  cards_.erase(unique(cards_.begin(), cards_.end()), cards_.end());
}

void Hand::enumerateAllHelper(
    vector<Hand>& hands,
    const Hand& currHand,
    size_t numCards,
    const Hand& deadCards) {
  if (numCards == 0) {
    hands.push_back(currHand);
    return;
  }

  for (size_t nextId = 1; nextId <= Card::MAX_ID; ++nextId) {
    Card nextCard(nextId);
    if (currHand.containsCard(nextCard) || deadCards.containsCard(nextCard)) {
      continue;
    }

    Hand nextHand = currHand;
    nextHand.addCard(nextCard);
    enumerateAllHelper(hands, nextHand, numCards - 1);
  }
}
