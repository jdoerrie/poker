#pragma once

#include <iostream>
#include <string>
#include <vector>

enum class GameType;

class Card;

class CardCollection {
 public:
  static const size_t MAX_ID;
  static const size_t INVALID_ID;

  CardCollection(const std::string& str = "");
  CardCollection(const std::vector<Card>& cards);
  CardCollection(size_t id);

  const std::vector<Card>& getCards() const;
  size_t getId() const;
  static std::vector<CardCollection> enumerateAllHands(GameType gameType);

  static std::vector<CardCollection> enumerateAllBoards(
    const CardCollection& initialBoard = {},
    const CardCollection& deadCards = {}
  );

  bool isValid(size_t numCards = 0) const;
  bool addCard(const Card& card);
  bool containsCard(const Card& card) const;

  std::string toString(bool allRanksFirst = false, bool useColor = false) const;
  friend std::ostream& operator<<(std::ostream& out, const CardCollection& hand) {
    out << hand.toString(/* allRanksFirst */ false, /* useColor */ true);
    return out;
  }

  bool operator<(const CardCollection& other) const;
  bool operator==(const CardCollection& other) const;

 private:
  size_t id_;
  std::vector<Card> cards_;

  void normalize();

  static void enumerateAllHelper(
      std::vector<CardCollection>& hands,
      const CardCollection& currHand,
      size_t numCards,
      const CardCollection& deadCards = {});
};

template<class Container>
void printFormatted(
  const Container& c,
  std::ostream& out  = std::cout,
  const std::string& padding = "  ",
  size_t lineLength = 80,
  const std::string& seperator = ", "
) {
  out << padding;
  size_t currLength = padding.size();
  for (const CardCollection& hand: c) {
    if (currLength + hand.toString().size() + seperator.size() > lineLength) {
      out << std::endl << padding;
      currLength = padding.size();
    }

    out << hand << seperator;
    currLength += hand.toString().size() + seperator.size();
  }

  out << std::endl;
}

namespace std {
  template<>
  struct hash<CardCollection> {
    size_t operator()(const CardCollection& hand) const {
      return hand.getId();
    }
  };
}
