#pragma once

#include <string>

enum class Category {
  Invalid         = 0,
  High_Card       = 1,
  Pair            = 2,
  Two_Pair        = 3,
  Three_of_a_Kind = 4,
  Straight        = 5,
  Flush           = 6,
  Full_House      = 7,
  Four_of_a_Kind  = 8,
  Straight_Flush  = 9,
};

std::string toString(Category category);
