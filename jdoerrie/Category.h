#pragma once

enum class Category {
  Invalid = 0,
  High_Card = 1,
  Pair = 2,
  Two_Pair = 3,
  Three_of_a_Kind = 4,
  Straight = 5,
  Flush = 6,
  Full_House = 7,
  Four_of_a_Kind = 8,
  Straight_Flush = 9,
};

constexpr const char* toString(Category category) {
  const char* categories[] = {
      "Invalid",         "High Card",     "Pair",  "Two Pair",
      "Three of a Kind", "Straight",      "Flush", "Full House",
      "Four of a Kind",  "Straight Flush"};

  return categories[static_cast<size_t>(category)];
};
