#pragma once

#include "GameType.h"
#include <string>
#include <vector>

class Card;

class Hand {
 public:
  static const size_t MAX_ID;
  static const size_t INVALID_ID;

  Hand(const std::string& str = "");
  Hand(const std::vector<Card>& cards);
  Hand(size_t id);

  const std::vector<Card>& getCards() const;
  size_t getId() const;
  static std::vector<Hand> enumerateAllHands(GameType gameType);

  static std::vector<Hand> enumerateAllBoards(
    const Hand& initialBoard = {},
    const Hand& deadCards = {}
  );

  bool isValid(size_t numCards = 0) const;
  bool addCard(const Card& card);
  bool containsCard(const Card& card) const;

  std::string toString(bool allRanksFirst = false, bool useColor = false) const;
  friend std::ostream& operator<<(std::ostream& out, const Hand& hand) {
    out << hand.toString(/* allRanksFirst */ false, /* useColor */ true);
    return out;
  }

  bool operator<(const Hand& other) const;
  bool operator==(const Hand& other) const;

 private:
  size_t id_;
  std::vector<Card> cards_;

  void normalize();

  static void enumerateAllHelper(
      std::vector<Hand>& hands,
      const Hand& currHand,
      size_t numCards,
      const Hand& deadCards = {});
};

namespace std {
  template<>
  struct hash<Hand> {
    size_t operator()(const Hand& hand) const {
      return hand.getId();
    }
  };
}
