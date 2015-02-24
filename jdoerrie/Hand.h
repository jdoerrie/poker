#pragma once

#include "GameType.h"
#include <string>
#include <vector>

class Card;

class Hand {
 public:
  Hand(const std::string& str = "");
  Hand(std::vector<Card> cards);
  Hand(size_t id);

  const std::vector<Card>& getCards() const;
  size_t getId() const;
  static std::vector<Hand> enumerateAllHands(GameType gameType);

  bool isValid(size_t numCards = 0) const;
  bool addCard(const Card& card);
  bool containsCard(const Card& card) const;

  std::string toString(bool allRanksFirst = false) const;
  friend std::ostream& operator<<(std::ostream& out, const Hand& hand) {
    out << hand.toString();
    return out;
  }

  bool operator<(const Hand& other) const;
  bool operator==(const Hand& other) const;

 private:
  std::vector<Card> cards_;

  void normalize();

  static void enumerateAllHandsHelper(
      std::vector<Hand>& hands,
      const Hand& currHand,
      size_t numCards);
};
