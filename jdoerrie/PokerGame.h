#pragma once

#include "Category.h"
#include "GameType.h"
#include "Hand.h"
#include "Range.h"
#include "Utils.h"
#include <vector>

class Evaluator;

class PokerGame {
 public:
  PokerGame(
      GameType gameType = GameType::HOLDEM,
      std::vector<Range> ranges = {},
      Hand board = {},
      Hand dead = {}
  );

  void printEquities() const;
  void printNextCards() const;
  void printRangeBreakdown() const;
  void printCategories() const;
  void printDraws(double minProb = 0.01) const;

  GameType getGameType() const;
  const std::vector<Range>& getRanges() const;
  const Hand& getBoard() const;
  const Hand& getDead() const;

  void setGameType(GameType gameType);
  void setRanges(std::vector<Range> ranges);
  void setBoard(Hand board);
  void setDead(Hand dead);

 private:
  GameType gameType_;
  std::vector<Range> ranges_;
  Hand board_;
  Hand dead_;
  static Evaluator evaluator_;
};
