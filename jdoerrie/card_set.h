#pragma once

#include <iostream>
#include <string>
#include <vector>

enum class GameType;

class Card;

class CardSet {
 public:
  // All bits from 1 to 52 are set
  constexpr static size_t MAX_ID = (1LL << 53) - 2;
  constexpr static size_t INVALID_ID = 1;

  CardSet() = default;
  CardSet(const std::string& str);
  CardSet(const std::vector<Card>& cards);
  explicit CardSet(size_t id);

  const std::vector<Card>& getCards() const;
  size_t GetId() const;
  static std::vector<CardSet> enumerateAllHands(GameType gameType);

  static std::vector<CardSet> enumerateAllBoards(
      const CardSet& initialBoard = {},
      const CardSet& deadCards = {});

  bool isValid(size_t numCards = 0) const;
  bool addCard(const Card& card);
  bool containsCard(const Card& card) const;

  std::string toString(bool allRanksFirst = false) const;
  friend std::ostream& operator<<(std::ostream& out, const CardSet& hand) {
    out << hand.toString(/* allRanksFirst */ false);
    return out;
  }

  bool operator<(const CardSet& other) const;
  bool operator==(const CardSet& other) const;

 private:
  size_t id_;
  std::vector<Card> cards_;

  void normalize();

  static void enumerateAllHelper(std::vector<CardSet>& hands,
                                 const CardSet& currHand,
                                 size_t numCards,
                                 const CardSet& deadCards = {});
};

template <class Container>
void printFormatted(const Container& c,
                    std::ostream& out = std::cout,
                    const std::string& padding = "  ",
                    size_t lineLength = 80,
                    const std::string& seperator = ", ") {
  out << padding;
  size_t currLength = padding.size();
  for (const CardSet& hand : c) {
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
template <>
struct hash<CardSet> {
  size_t operator()(const CardSet& hand) const { return hand.GetId(); }
};
}
