#pragma once

#include "Card.h"

namespace poker {

struct OrderCardsByRank {
  constexpr bool operator()(Card lhs, Card rhs) const {
    return lhs.getRank() < rhs.getRank() ||
           (!(rhs.getRank() < lhs.getRank()) && lhs.getSuit() < rhs.getSuit());
  }
};

struct OrderCardsBySuit {
  constexpr bool operator()(Card lhs, Card rhs) const {
    return lhs.getSuit() < rhs.getSuit() ||
           (!(rhs.getSuit() < lhs.getSuit()) && lhs.getRank() < rhs.getRank());
  }
};

struct OrderCardsByRankRev {
  constexpr bool operator()(Card lhs, Card rhs) const {
    return lhs.getRank() > rhs.getRank() ||
           (!(rhs.getRank() > lhs.getRank()) && lhs.getSuit() > rhs.getSuit());
  }
};

struct OrderCardsBySuitRev {
  constexpr bool operator()(Card lhs, Card rhs) const {
    return lhs.getSuit() > rhs.getSuit() ||
           (!(rhs.getSuit() > lhs.getSuit()) && lhs.getRank() > rhs.getRank());
  }
};

} // poker
