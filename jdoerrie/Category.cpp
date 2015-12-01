#include "Category.h"

#include <array>

std::string toString(Category category) {
  static const std::array<std::string, 10> categories = {{
    "Invalid",
    "High Card",
    "Pair",
    "Two Pair",
    "Three of a Kind",
    "Straight",
    "Flush",
    "Full House",
    "Four of a Kind",
    "Straight Flush"
  }};

  return categories[static_cast<size_t>(category)];
}

