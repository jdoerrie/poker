#pragma once

#include <cstdint>
#include <vector>

namespace poker {
namespace rank {
enum class Rank {
  INVALID = 0,
  TWO = 2,
  THREE = 3,
  FOUR = 4,
  FIVE = 5,
  SIX = 6,
  SEVEN = 7,
  EIGHT = 8,
  NINE = 9,
  TEN = 10,
  JACK = 11,
  QUEEN = 12,
  KING = 13,
  ACE = 14,
};

constexpr Rank fromChar(char c) {
  switch (c) {
    case '2':
      return Rank::TWO;

    case '3':
      return Rank::THREE;

    case '4':
      return Rank::FOUR;

    case '5':
      return Rank::FIVE;

    case '6':
      return Rank::SIX;

    case '7':
      return Rank::SEVEN;

    case '8':
      return Rank::EIGHT;

    case '9':
      return Rank::NINE;

    case 't':
    case 'T':
      return Rank::TEN;

    case 'j':
    case 'J':
      return Rank::JACK;

    case 'q':
    case 'Q':
      return Rank::QUEEN;

    case 'k':
    case 'K':
      return Rank::KING;

    case 'a':
    case 'A':
      return Rank::ACE;

    default:
      return Rank::INVALID;
  }
}

constexpr char toChar(Rank rank) {
  constexpr char ranks[] = "--23456789TJQKA";
  return ranks[size_t(rank)];
}

constexpr size_t getHash(Rank rank) {
  size_t bit = static_cast<size_t>(rank);
  return 1uLL << bit
       | 1uLL << (16 + bit)
       | 1uLL << (32 + bit)
       | 1uLL << (48 + bit);
}

constexpr bool isValid(Rank rank) { return rank != Rank::INVALID; }

template <class Container = std::vector<Rank>>
constexpr Container getAllRanks() {
  Container c;
  for (int i = int(Rank::TWO); i <= int(Rank::ACE); ++i) {
    c.insert(c.end(), Rank(i));
  }

  return c;
}
}  // namespace rank
}  // namespace poker
