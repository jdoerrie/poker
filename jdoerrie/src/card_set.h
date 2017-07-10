#pragma once

#include <bitset>
#include <iostream>
#include <string>
#include <vector>

#include "GameType.h"
#include "card.h"

class CardSet {
 public:
  static constexpr size_t N_BITS = Card::MAX_ID + 1;
  static constexpr size_t MAX_ID = (1LL << N_BITS) - 1;
  static constexpr size_t INVALID_ID = 1 << Card::INVALID_ID;

  CardSet() = default;
  explicit CardSet(const Card& card);
  explicit CardSet(const std::bitset<N_BITS>& bits);
  explicit CardSet(const std::string& str);
  explicit CardSet(const std::vector<Card>& cards);
  explicit CardSet(size_t id);

  bool empty() const { return bits_ == 0; }
  size_t size() const { return bits_.count(); }
  size_t id() const { return bits_.to_ullong(); }

  bool IsValid(size_t num_cards = 0) const;
  bool Add(const Card& card);
  CardSet& Add(const CardSet& other);
  CardSet& Remove(const Card& card);
  CardSet& Remove(const CardSet& other);
  bool Contains(const Card& card) const;
  bool Contains(const CardSet& other) const;

  CardSet& operator&=(const CardSet& other);
  CardSet& operator|=(const CardSet& other);
  CardSet& operator^=(const CardSet& other);
  CardSet& operator-=(const CardSet& other);
  CardSet operator~() const;

  std::vector<int> ToIds() const;
  std::vector<Card> ToCards() const;
  std::string ToString(bool allRanksFirst = false) const;

 private:
  std::bitset<N_BITS> bits_;
};

CardSet operator&(const CardSet& lhs, const CardSet& rhs);
CardSet operator|(const CardSet& lhs, const CardSet& rhs);
CardSet operator^(const CardSet& lhs, const CardSet& rhs);
CardSet operator-(const CardSet& lhs, const CardSet& rhs);

bool operator<(const CardSet& lhs, const CardSet& rhs);
bool operator==(const CardSet& lhs, const CardSet& rhs);
bool operator<=(const CardSet& lhs, const CardSet& rhs);
bool operator>=(const CardSet& lhs, const CardSet& rhs);
std::ostream& operator<<(std::ostream& out, const CardSet& hand);

std::vector<CardSet> EnumerateAllHands(GameType gameType,
                                       const CardSet& deadCards = {});
std::vector<CardSet> EnumerateAllBoards(const CardSet& initialBoard = {},
                                        const CardSet& deadCards = {});

void printFormatted(const std::vector<CardSet>& cards,
                    std::ostream& out = std::cout,
                    const std::string& padding = "  ",
                    size_t lineLength = 80,
                    const std::string& seperator = ", ");

namespace std {
template <>
struct hash<CardSet> {
  size_t operator()(const CardSet& hand) const { return hand.id(); }
};
}
