#pragma once

#include <array>

class Suit {
 private:
  enum class kSuit {
    NONE,
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES,
  };

  kSuit suit_ = kSuit::NONE;

 public:
  constexpr Suit() = default;
  constexpr Suit(size_t id) : suit_{static_cast<kSuit>(id)} {}
  constexpr Suit(char c) {
    switch (c) {
      case 'c':
      case 'C':
        suit_ = kSuit::CLUBS;
        return;
      case 'd':
      case 'D':
        suit_ = kSuit::DIAMONDS;
        return;
      case 'h':
      case 'H':
        suit_ = kSuit::HEARTS;
        return;
      case 's':
      case 'S':
        suit_ = kSuit::SPADES;
        return;
      default:
        suit_ = kSuit::NONE;
    }
  }

  constexpr static auto MIN_ID = static_cast<size_t>(kSuit::CLUBS);
  constexpr static auto MAX_ID = static_cast<size_t>(kSuit::SPADES);

  constexpr auto id() const { return static_cast<size_t>(suit_); }
  constexpr auto IsValid() const { return suit_ != kSuit::NONE; }
  constexpr auto ToChar() const {
    switch (suit_) {
      case kSuit::CLUBS:
        return 'c';
      case kSuit::DIAMONDS:
        return 'd';
      case kSuit::HEARTS:
        return 'h';
      case kSuit::SPADES:
        return 's';
      default:
        return '?';
    }
  }

  constexpr auto ToUTF8() {
    switch (suit_) {
      case kSuit::CLUBS:
        return u8"♣";
      case kSuit::DIAMONDS:
        return u8"♦";
      case kSuit::HEARTS:
        return u8"♥";
      case kSuit::SPADES:
        return u8"♠";
      default:
        return "-";
    }
  }
};

namespace suit {

constexpr std::array<Suit, Suit::MAX_ID> GetAllValidSuits() {
  return {{Suit{'c'}, Suit{'d'}, Suit{'h'}, Suit{'s'}}};
}

}  // namespace suit
