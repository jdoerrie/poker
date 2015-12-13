#pragma once

#include "Card.h"
#include "Rank.h"
#include "Suit.h"

#include <type_traits>

namespace poker {
enum class PrintOptions {
  NONE = 0x00,
  USE_BASH_COLORS = 0x01,
  ALL_RANKS_FIRST = 0x02,
};

using T = std::underlying_type_t<PrintOptions>;
constexpr PrintOptions& operator&=(PrintOptions& lhs, PrintOptions rhs) {
  return lhs = static_cast<PrintOptions>(static_cast<T>(lhs) &
                                         static_cast<T>(rhs));
}

constexpr PrintOptions& operator|=(PrintOptions& lhs, PrintOptions rhs) {
  return lhs = static_cast<PrintOptions>(static_cast<T>(lhs) |
                                         static_cast<T>(rhs));
}

constexpr PrintOptions operator&(PrintOptions lhs, PrintOptions rhs) {
  return lhs &= rhs;
}

constexpr PrintOptions operator|(PrintOptions lhs, PrintOptions rhs) {
  return lhs |= rhs;
}

constexpr bool toBool(PrintOptions options) {
  return options != PrintOptions::NONE;
}


constexpr auto resetBashColor = "\x1B[0m";
constexpr const char* getBashColor(Suit suit) {
  const char* bashColors[] = {
      "\x1B[0m",
      "\x1B[0;32m",  // Clubs    -> Green
      "\x1B[0;34m",  // Diamonds -> Blue
      "\x1B[0;31m",  // Hearts   -> Red
      "\x1B[0;37m"   // Spades   -> White
  };

  return bashColors[static_cast<size_t>(suit)];
}

std::string toString(Card card, PrintOptions options = PrintOptions::NONE) {
  std::string str{toChar(card.getRank()), toChar(card.getSuit())};
  if (toBool(options & PrintOptions::USE_BASH_COLORS)) {
    str = getBashColor(card.getSuit()) + str + resetBashColor;
  }

  return str;
}

std::ostream& operator<<(std::ostream& out, Card card) {
  return out << toString(card, PrintOptions::USE_BASH_COLORS);
}

}  // namespace poker
