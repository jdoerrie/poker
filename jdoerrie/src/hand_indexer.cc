#include "hand_indexer.h"

#include <numeric>

#include "card_set.h"

HandIndexer::HandIndexer(const std::vector<uint8_t>& cards_per_round)
    : cards_per_round_{cards_per_round} {
  hand_indexer_init(cards_per_round_.size(), cards_per_round_.data(),
                    &hand_indexer_);
}

size_t HandIndexer::size() const {
  return hand_indexer_size(&hand_indexer_, cards_per_round_.size() - 1);
}

size_t HandIndexer::IndexHands(const std::vector<CardSet>& hands) const {
  std::vector<uint8_t> cards;
  for (const auto& hand : hands) {
    for (const auto& card : hand.ToCards()) {
      auto suit = std::string(SUIT_TO_CHAR).find(card.suit().ToChar());
      auto rank = std::string(RANK_TO_CHAR).find(to_char(card.rank()));
      cards.push_back(deck_make_card(suit, rank));
    }
  }

  return hand_index_last(&hand_indexer_, cards.data());
}

std::vector<CardSet> HandIndexer::UnindexHands(size_t index) const {
  std::vector<CardSet> hands;
  auto rounds = hand_indexer_.rounds;
  std::vector<uint8_t> cards(std::accumulate(std::begin(cards_per_round_),
                                             std::end(cards_per_round_), 0));
  if (!hand_unindex(&hand_indexer_, rounds - 1, index, cards.data())) {
    return {};
  }

  CardSet hand;
  auto round = 0;
  for (auto card : cards) {
    char rank = RANK_TO_CHAR[deck_get_rank(card)];
    char suit = SUIT_TO_CHAR[deck_get_suit(card)];
    hand.Add({to_rank(rank), Suit{suit}});

    if (hand.size() == cards_per_round_[round]) {
      hands.push_back(std::move(hand));
      ++round;
      hand = {};
    }
  }

  return hands;
}

HandIndexer::~HandIndexer() {
  hand_indexer_free(&hand_indexer_);
}
