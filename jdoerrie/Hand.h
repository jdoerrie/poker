#pragma once

#include "Card.h"
#include "GameType.h"
#include "Rank.h"
#include "Suit.h"

#include <iostream>
#include <numeric>
#include <string>
#include <cassert>
#include <vector>

namespace poker {
template <GameType Game>
class Hand {
 private:
  static constexpr size_t numCards_{getNumCards(Game)};
  std::array<Card, numCards_> cards_;

 public:
  constexpr explicit Hand(const std::array<Card, numCards_>& cards)
      : cards_(cards) {}
  explicit Hand(const std::string& str) {
    assert(str.size() == 2 * numCards_);

    for (size_t i = 0; i < numCards_; ++i) {
      cards_[i] = Card(str[2 * i], str[2 * i + 1]);
    }
  }

  constexpr const std::array<Card, numCards_>& getCards() const {
    return cards_;
  }
};

template <GameType Game>
constexpr size_t getHash(const Hand<Game>& hand) {
  size_t result = 0;
  std::for_each(begin(hand.getCards()), end(hand.getCards()),
                [&](Card card) { result |= getHash(card); });
  return result;
}
}

// namespace std {
// template <>
// struct hash<Hand> {
//   size_t operator()(const Hand& hand) const { return hand.getId(); }
// };
// }
