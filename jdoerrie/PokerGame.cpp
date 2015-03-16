#include "PokerGame.h"

#include "Evaluator.h"
#include "Equity.h"
#include "Card.h"
#include <algorithm>
#include <iostream>

using namespace std;

PokerGame::PokerGame(
    GameType gameType,
    vector<Range> ranges,
    Hand board,
    Hand dead
) : gameType_(gameType), ranges_(ranges), board_(board), dead_(dead) {
  if (!Evaluator::initialize("../linux/HandRanks.dat")) {
    cerr << "Failed to initialize evaluator, exiting." << endl;
    exit(1);
  }
}

void PokerGame::printEquities() const {
  if (ranges_.size() < 2) {
    cerr << "Error: Need at least two players" << endl;
    return;
  }

  auto results = evaluator_.evalRanges(gameType_, ranges_, board_, dead_);
  cout << "Percentage Wins Losses Ties" << endl;
  for (const auto& result: results) {
    cout << fixed << Utils::colorEquity(result);
    cout << " " << result.getGameResults()[1];
    cout << " " << result.getGameResults()[0];
    for (size_t i = 2; i < result.getGameResults().size(); ++i) {
      cout << " " << result.getGameResults()[i];
    }

    cout << endl;
  }
}

void PokerGame::printNextCards() const {
  if (ranges_.size() < 2) {
    cerr << "Error: Need at least two players" << endl;
    return;
  }

  typedef pair<Equity, Card> Result;
  vector<Result> results;

  for (const auto& card: Card::enumerateAllCards()) {
    if (board_.containsCard(card) || dead_.containsCard(card)) {
      continue;
    }

    Hand newBoard = board_;
    newBoard.addCard(card);

    auto result = evaluator_.evalRanges(gameType_, ranges_, newBoard, dead_);
    if (result[0].getNumGames() != 0) {
      results.emplace_back(result[0], card);
    }
  }

  sort(results.rbegin(), results.rend());
  cout << "Best Cards for Player 1:";
  for (size_t i = 0; i < results.size(); ++i) {
    if (i > 0 && results[i-1].first.toDouble() == results[i].first.toDouble()) {
      cout << ", " << results[i].second;
    } else {
      cout << endl;
      cout << Utils::colorEquity(results[i].first) << ": " << results[i].second;
    }
  }

  cout << endl;
}

void PokerGame::printRangeBreakdown() const {
  if (ranges_.size() < 2) {
    cerr << "Error: Need at least two players" << endl;
    return;
  }

  typedef pair<Equity, Hand> Result;
  vector<Result> results;

  for (const auto& hand: ranges_[0].getHands()) {
    Range heroRange = vector<Hand>({hand});
    vector<Range> newRanges = ranges_;
    newRanges[0] = heroRange;

    auto result = evaluator_.evalRanges(gameType_, newRanges, board_, dead_);
    if (result[0].getNumGames() != 0) {
      results.emplace_back(result[0], hand);
    }
  }

  sort(results.rbegin(), results.rend());
  cout << "Best Hands for Player 1:";
  for (size_t i = 0; i < results.size(); ++i) {
    if (i > 0 && results[i-1].first.toDouble() == results[i].first.toDouble()) {
      cout << ", " << results[i].second.toString();
    } else {
      cout << endl;
      cout << Utils::colorEquity(results[i].first) << ": "
           << results[i].second.toString();
    }
  }

  cout << endl;
}



GameType PokerGame::getGameType() const {
  return gameType_;
}
const vector<Range>& PokerGame::getRanges() const {
  return ranges_;
}

const Hand& PokerGame::getBoard() const {
  return board_;
}

const Hand& PokerGame::getDead() const {
  return dead_;
}

void PokerGame::setGameType(GameType gameType) {
  gameType_ = std::move(gameType);
}

void PokerGame::setRanges(vector<Range> ranges) {
  ranges_ = std::move(ranges);
}

void PokerGame::setBoard(Hand board) {
  board_ = std::move(board);
}

void PokerGame::setDead(Hand dead) {
  dead_ = std::move(dead);
}
