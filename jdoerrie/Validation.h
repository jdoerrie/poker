#pragma once

#include "Card.h"
#include "GameType.h"
#include "Hand.h"
#include "Rank.h"
#include "Suit.h"

namespace poker {
constexpr bool isValid(GameType gameType) {
  return gameType != GameType::INVALID;
}

constexpr bool isValid(Rank rank) { return rank != Rank::INVALID; }

constexpr bool isValid(Suit suit) { return suit != Suit::INVALID; }

constexpr bool isValid(Card card) {
  return isValid(card.getRank()) && isValid(card.getSuit());
}

template <GameType Game>
constexpr bool isValid(const Hand<Game>& hand) {
  const auto& cards = hand.getCards();
  // Cards need to be all valid and also unique.
  for (auto curr = std::begin(cards); curr != std::end(cards); ++curr) {
    if (!isValid(*curr) || std::find(std::begin(cards), curr, *curr) != curr) {
      return false;
    }
  }

  return true;
}
} // namespace poker
