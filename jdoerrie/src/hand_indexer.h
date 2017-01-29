#pragma once

extern "C" {
#define _Bool bool
#include "hand-isomorphism/src/hand_index.h"
}

#include <vector>

class CardSet;

class HandIndexer {
 public:
  HandIndexer(const std::vector<uint8_t>& cards_per_round);
  ~HandIndexer();

  size_t size() const;
  size_t IndexHands(const std::vector<CardSet>& hands) const;
  std::vector<CardSet> UnindexHands(size_t index) const;

 private:
  hand_indexer_t hand_indexer_;
  std::vector<uint8_t> cards_per_round_;
};
