#pragma once

#include <array>

class Rank {
 private:
  enum class kRank {
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

  kRank rank_ = kRank::NONE;

 public:
  constexpr Rank() = default;
  constexpr explicit Rank(size_t id) : rank_{static_cast<kRank>(id)} {}
  constexpr explicit Rank(char c) {
    switch (c) {
      case '2':
        rank_ = kRank::TWO;
        return;
      case '3':
        rank_ = kRank::THREE;
        return;
      case '4':
        rank_ = kRank::FOUR;
        return;
      case '5':
        rank_ = kRank::FIVE;
        return;
      case '6':
        rank_ = kRank::SIX;
        return;
      case '7':
        rank_ = kRank::SEVEN;
        return;
      case '8':
        rank_ = kRank::EIGHT;
        return;
      case '9':
        rank_ = kRank::NINE;
        return;
      case 't':
      case 'T':
        rank_ = kRank::TEN;
        return;
      case 'j':
      case 'J':
        rank_ = kRank::JACK;
        return;
      case 'q':
      case 'Q':
        rank_ = kRank::QUEEN;
        return;
      case 'k':
      case 'K':
        rank_ = kRank::KING;
        return;
      case 'a':
      case 'A':
        rank_ = kRank::ACE;
        return;
      default:
        rank_ = kRank::NONE;
    }
  }

  constexpr static auto MIN_ID = static_cast<size_t>(kRank::TWO);
  constexpr static auto MAX_ID = static_cast<size_t>(kRank::ACE);

  constexpr auto id() const { return static_cast<size_t>(rank_); }
  constexpr auto IsValid() const { return rank_ != kRank::NONE; }
  constexpr auto ToChar() const {
    switch (rank_) {
      case kRank::TWO:
        return '2';
      case kRank::THREE:
        return '3';
      case kRank::FOUR:
        return '4';
      case kRank::FIVE:
        return '5';
      case kRank::SIX:
        return '6';
      case kRank::SEVEN:
        return '7';
      case kRank::EIGHT:
        return '8';
      case kRank::NINE:
        return '9';
      case kRank::TEN:
        return 'T';
      case kRank::JACK:
        return 'J';
      case kRank::QUEEN:
        return 'Q';
      case kRank::KING:
        return 'K';
      case kRank::ACE:
        return 'A';
      default:
        return '?';
    }
  }
};

namespace rank {

constexpr std::array<Rank, Rank::MAX_ID> GetAllValidRanks() {
  return {{Rank{'2'}, Rank{'3'}, Rank{'4'}, Rank{'5'}, Rank{'6'}, Rank{'7'},
           Rank{'8'}, Rank{'9'}, Rank{'T'}, Rank{'J'}, Rank{'Q'}, Rank{'K'},
           Rank{'A'}}};
}

}  // namespace rank
