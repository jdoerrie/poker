#pragma once

#include "Card.h"
#include "Rank.h"
#include "Suit.h"

namespace poker {

constexpr size_t getHash(Rank rank) {
  size_t base = 1;
  size_t bit = static_cast<size_t>(rank);

  return base << bit | base << (16 + bit) | base << (32 + bit) |
         base << (48 + bit);
}

constexpr size_t getHash(Suit suit) {
  return size_t(0xFFFF) << (16 * (size_t(suit) - 1));
}

constexpr size_t getHash(Card card) {
  return getHash(card.getRank()) & getHash(card.getSuit());
}

}  // poker

namespace std {

using namespace poker;

template <>
struct hash<Rank> {
  size_t operator()(Rank rank) const { return getHash(rank); }
};

template <>
struct hash<Suit> {
  size_t operator()(Suit suit) const { return getHash(suit); }
};

template <>
struct hash<Card> {
  size_t operator()(Card card) const { return getHash(card); }
};

}  // std
