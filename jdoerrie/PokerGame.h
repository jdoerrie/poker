#pragma once

#include "GameType.h"
#include "card_set.h"

#include <vector>

class Evaluator;
class Range;

class PokerGame {
 public:
  PokerGame(
      GameType gameType = GameType::HOLDEM,
      std::vector<Range> ranges = {},
      CardSet board = {},
      CardSet dead = {}
  );

  void printEquities() const;
  void printNextCards() const;
  void printRangeBreakdown() const;
  void printCategories() const;
  void printDraws(double minProb = 0.01) const;

  GameType getGameType() const;
  const std::vector<Range>& getRanges() const;
  const CardSet& getBoard() const;
  const CardSet& getDead() const;

  void setGameType(GameType gameType);
  void setRanges(std::vector<Range> ranges);
  void setBoard(CardSet board);
  void setDead(CardSet dead);

 private:
  GameType gameType_;
  std::vector<Range> ranges_;
  CardSet board_;
  CardSet dead_;
  static Evaluator evaluator_;
};
