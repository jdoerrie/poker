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
  enumerateAllHandsHelper(hands, {}, Utils::getNumCards(gameType));
  sort(hands.begin(), hands.end());
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

string Hand::toString(bool allRanksFirst) const {
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
      str += card.toString();
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

void Hand::enumerateAllHandsHelper(
    vector<Hand>& hands,
    const Hand& currHand,
    size_t numCards) {
  if (numCards == 0) {
    hands.push_back(currHand);
    return;
  }

  int minId = currHand.getCards().empty()
    ? 1
    : currHand.getCards()[0].getId() + 1;

  for (size_t nextId = minId; nextId <= Card::MAX_ID; ++nextId) {
    Hand nextHand = currHand;
    nextHand.addCard(Card(nextId));
    enumerateAllHandsHelper(hands, nextHand, numCards - 1);
  }
}

template<>
struct hash<Hand> {
  size_t operator()(const Hand& hand) const {
    return hand.getId();
  }
};

