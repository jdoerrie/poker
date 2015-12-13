#pragma once

#include "Card.h"
#include "Rank.h"
#include "Suit.h"

#include <cassert>
#include <numeric>
#include <vector>

namespace poker {

template <class Container = std::vector<Rank>>
constexpr Container getAllRanks() {
  Container c;
  for (int i = int(Rank::TWO); i <= int(Rank::ACE); ++i) {
    c.insert(c.end(), Rank(i));
  }

  return c;
}

template <class Container = std::vector<Suit>>
constexpr Container getAllSuits() {
  Container c;
  for (int i = int(Suit::CLUBS); i <= int(Suit::SPADES); ++i) {
    c.emplace(c.end(), Suit(i));
  }

  return c;
}

template <class Container = std::vector<Card>>
Container getAllCards() {
  Container c;
  for (auto rank : getAllRanks()) {
    for (auto suit : getAllSuits()) {
      c.emplace_back(rank, suit);
    }
  }

  return c;
}

template <class Entry, class Container = std::vector<Entry>>
Container getAllCombinations(const Entry& entries, size_t K = 1) {
  size_t N = entries.size();
  assert(K <= N);

  auto combos = Container();
  if (K == 0) {
    return combos;
  }

  std::vector<size_t> indices(K);
  std::iota(std::begin(indices), std::end(indices), 0);

  while (true) {
    Entry entry;
    // Restricts possible types, but results in big speed up for vector (~3x).
    entry.reserve(K);
    for (size_t i = 0; i < K; ++i) {
      entry.insert(std::end(entry), entries[indices[i]]);
    }

    combos.insert(std::end(combos), std::move(entry));

    // Increment current set of indices, pay attention to upper bound and
    // that indices increase monotonically.
    size_t revIdx = 0;
    ++indices[K - 1 - revIdx];
    while (revIdx < K && N - revIdx <= indices[K - 1 - revIdx]) {
      ++revIdx;
      ++indices[K - 1 - revIdx];
    }

    // Terminating condition, implies indices == (N-K, N].
    if (revIdx == K) {
      break;
    }
    while (revIdx >= 1) {
      indices[K - revIdx] = indices[K - 1 - revIdx] + 1;
      --revIdx;
    }
  }

  return combos;
}

}
