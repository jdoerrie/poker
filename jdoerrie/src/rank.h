#ifndef POKER_RANK_H_
#define POKER_RANK_H_

#include <array>

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

constexpr Rank to_rank(char c) {
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
};

constexpr char to_char(Rank rank) {
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
        return '?';
  }
}

constexpr size_t to_id(Rank rank) {
  return static_cast<size_t>(rank);
}

constexpr Rank to_rank(size_t id) {
  return static_cast<Rank>(id);
}

constexpr bool is_valid(Rank rank) {
  return rank != Rank::NONE;
}

constexpr size_t MIN_RANK_ID = static_cast<size_t>(Rank::TWO);
constexpr size_t MAX_RANK_ID = static_cast<size_t>(Rank::ACE);


namespace rank {

constexpr std::array<Rank, MAX_RANK_ID> all_ranks() {
  return {{
    to_rank('2'),
    to_rank('3'),
    to_rank('4'),
    to_rank('5'),
    to_rank('6'),
    to_rank('7'),
    to_rank('8'),
    to_rank('9'),
    to_rank('T'),
    to_rank('J'),
    to_rank('Q'),
    to_rank('K'),
    to_rank('A'),
  }};
}

}  // namespace rank

#endif // ifndef POKER_RANK_H_
