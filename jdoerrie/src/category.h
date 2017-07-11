#ifndef POKER_CATEGORY_H_
#define POKER_CATEGORY_H_

enum class Category {
  INVALID = 0,
  HIGH_CARD = 1,
  ONE_PAIR = 2,
  TWO_PAIR = 3,
  THREE_OF_A_KIND = 4,
  STRAIGHT = 5,
  FLUSH = 6,
  FULL_HOUSE = 7,
  FOUR_OF_A_KIND = 8,
  STRAIGHT_FLUSH = 9,
  ROYAL_FLUSH = 10,
};

constexpr const char* ToString(Category category) {
  switch (category) {
    case Category::HIGH_CARD:
      return "High Card";
    case Category::ONE_PAIR:
      return "One Pair";
    case Category::TWO_PAIR:
      return "Two Pair";
    case Category::THREE_OF_A_KIND:
      return "Three of a Kind";
    case Category::STRAIGHT:
      return "Straight";
    case Category::FLUSH:
      return "Flush";
    case Category::FULL_HOUSE:
      return "Full House";
    case Category::FOUR_OF_A_KIND:
      return "Four of a Kind";
    case Category::STRAIGHT_FLUSH:
      return "Straight Flush";
    case Category::ROYAL_FLUSH:
      return "Royal Flush";
    default:
      return "Invalid";
  }
}

#endif  // POKER_CATEGORY_H_
