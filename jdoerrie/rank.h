#pragma once

#include <cstddef>

enum class Rank {
  NONE,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  TEN,
  JACK,
  QUEEN,
  KING,
  ACE,
};

constexpr Rank ToRank(char c) {
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
      return Rank::NONE;
  }
}

constexpr char ToChar(Rank rank) {
  switch (rank) {
    case Rank::TWO:
      return '2';
    case Rank::THREE:
      return '3';
    case Rank::FOUR:
      return '4';
    case Rank::FIVE:
      return '5';
    case Rank::SIX:
      return '6';
    case Rank::SEVEN:
      return '7';
    case Rank::EIGHT:
      return '8';
    case Rank::NINE:
      return '9';
    case Rank::TEN:
      return 'T';
    case Rank::JACK:
      return 'J';
    case Rank::QUEEN:
      return 'Q';
    case Rank::KING:
      return 'K';
    case Rank::ACE:
      return 'A';
    default:
      return '-';
  }
}

constexpr size_t RANK_MIN_ID = static_cast<size_t>(Rank::TWO);
constexpr size_t RANK_MAX_ID = static_cast<size_t>(Rank::ACE);
