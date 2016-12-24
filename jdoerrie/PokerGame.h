#pragma once

#include "GameType.h"
#include "card_collection.h"

#include <vector>

class Evaluator;
class Range;

class PokerGame {
 public:
  PokerGame(
      GameType gameType = GameType::HOLDEM,
      std::vector<Range> ranges = {},
      CardCollection board = {},
      CardCollection dead = {}
  );

  void printEquities() const;
  void printNextCards() const;
  void printRangeBreakdown() const;
  void printCategories() const;
  void printDraws(double minProb = 0.01) const;

  GameType getGameType() const;
  const std::vector<Range>& getRanges() const;
  const CardCollection& getBoard() const;
  const CardCollection& getDead() const;

  void setGameType(GameType gameType);
  void setRanges(std::vector<Range> ranges);
  void setBoard(CardCollection board);
  void setDead(CardCollection dead);

 private:
  GameType gameType_;
  std::vector<Range> ranges_;
  CardCollection board_;
  CardCollection dead_;
  static Evaluator evaluator_;
};
