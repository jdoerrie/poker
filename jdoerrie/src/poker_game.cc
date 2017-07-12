#include "poker_game.h"

#include <algorithm>
#include <iostream>
#include <map>

#include "card.h"
#include "category.h"
#include "equity.h"
#include "evaluator.h"
#include "range.h"

using namespace std;

PokerGame::PokerGame(GameType gameType,
                     vector<Range> ranges,
                     CardSet board,
                     CardSet dead)
    : gameType_(gameType), ranges_(ranges), board_(board), dead_(dead) {
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
  for (const auto& result : results) {
    cout << fixed << colorEquity(result);
    cout << " " << result.wins();
    cout << " " << result.losses();
    for (size_t i = 2; i <= result.num_ties(); ++i) {
      cout << " " << result.ties(i);
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

  for (const auto& card : card::EnumerateAllCards()) {
    if (board_.Contains(card) || dead_.Contains(card)) {
      continue;
    }

    CardSet newBoard = board_;
    newBoard.Add(card);

    auto result = evaluator_.evalRanges(gameType_, ranges_, newBoard, dead_);
    if (result[0].num_games() != 0) {
      results.emplace_back(result[0], card);
    }
  }

  sort(results.rbegin(), results.rend());
  cout << "Best Cards for Player 1:";
  for (size_t i = 0; i < results.size(); ++i) {
    if (i > 0 &&
        results[i - 1].first.ToDouble() == results[i].first.ToDouble()) {
      cout << ", " << results[i].second;
    } else {
      cout << endl;
      cout << colorEquity(results[i].first) << ": " << results[i].second;
    }
  }

  cout << endl;
}

void PokerGame::printRangeBreakdown() const {
  if (ranges_.size() < 2) {
    cerr << "Error: Need at least two players" << endl;
    return;
  }

  typedef pair<Equity, CardSet> Result;
  vector<Result> results;

  for (const auto& hand : ranges_[0].getHands()) {
    Range heroRange = vector<CardSet>({hand});
    vector<Range> newRanges = ranges_;
    newRanges[0] = heroRange;

    auto result = evaluator_.evalRanges(gameType_, newRanges, board_, dead_);
    if (result[0].num_games() != 0) {
      results.emplace_back(result[0], hand);
    }
  }

  map<double, vector<CardSet>, std::greater<double>> equityHandsMap;
  for (auto& result : results) {
    equityHandsMap[result.first.ToDouble()].push_back(std::move(result.second));
  }

  cout << "Best Hands for Player 1:\n";
  for (auto& equityHands : equityHandsMap) {
    std::cout << colorEquity(equityHands.first) << ":\n";
    std::sort(rbegin(equityHands.second), rend(equityHands.second));
    printFormatted(equityHands.second);
  }
}

void PokerGame::printCategories() const {
  if (getNumCards(gameType_) + board_.ToCards().size() < 5) {
    cout << "Error: Need at least 5 cards for classification." << endl;
    return;
  }

  size_t totalCounts = 0;
  map<Category, vector<pair<int, CardSet>>> categories;
  for (const auto& hand : ranges_[0].getHands()) {
    int handRank =
        Evaluator::getHandRank(CardSet(board_.ToString() + hand.ToString()));
    if (handRank == 0) {
      continue;
    }

    ++totalCounts;
    categories[static_cast<Category>(handRank >> 12)].emplace_back(handRank,
                                                                   hand);
  }

  for (const auto& category : categories) {
    cout << ToString(category.first) << ": ";
    auto hands = category.second;
    sort(hands.rbegin(), hands.rend());
    // cout << hands.size() << "/" << totalCounts << " "
    //      << hands.size() / static_cast<double>(totalCounts) << " ";
    for (auto hand : hands) {
      cout << hand.second << (hand == *(hands.rbegin()) ? "\n" : " ");
    }
  }
}

void PokerGame::printDraws(double minProb) const {
  if (getNumCards(gameType_) + board_.ToCards().size() < 5) {
    cerr << "Error: Need at least 5 cards for classification." << endl;
    return;
  }

  if (minProb < 0.0 || minProb > 1.0) {
    cerr << "Received invalid value for minimum draw probability, "
            "clamping to [0, 1]";
    minProb = max(0.0, min(1.0, minProb));
  }

  map<Category, vector<pair<double, CardSet>>> draws;
  auto allBoards = EnumerateAllBoards(board_);

  for (const auto& hand : ranges_[0].getHands()) {
    bool isInvalid = false;
    for (auto card : hand.ToCards()) {
      isInvalid |= board_.Contains(card);
      isInvalid |= dead_.Contains(card);
      if (isInvalid) {
        break;
      }
    }

    if (isInvalid) {
      continue;
    }

    int handRank =
        Evaluator::getHandRank(CardSet(board_.ToString() + hand.ToString()));

    vector<size_t> distribution(10);
    size_t totalCount = 0;
    size_t handRankTotal = 0;
    for (const auto& board : allBoards) {
      CardSet thisHand(board.ToString() + hand.ToString());
      bool isInvalid = false;
      for (auto card : hand.ToCards()) {
        isInvalid |= board.Contains(card);
        if (isInvalid) {
          break;
        }
      }

      if (isInvalid) {
        continue;
      }

      int thisHandRank = Evaluator::getHandRank(thisHand);

      ++distribution[static_cast<size_t>(thisHandRank >> 12)];
      ++totalCount;
      handRankTotal += thisHandRank;
    }

    for (int category = 0; category < 10; ++category) {
      if ((handRank >> 12) != category &&
          distribution[category] / static_cast<double>(totalCount) >= minProb) {
        draws[static_cast<Category>(category)].emplace_back(
            handRankTotal / static_cast<double>(totalCount), hand);
      }
    }
  }

  for (auto& draw : draws) {
    sort(draw.second.rbegin(), draw.second.rend());
  }

  vector<Category> drawCategories = {Category::STRAIGHT, Category::FLUSH,
                                     Category::STRAIGHT_FLUSH};

  for (auto drawCategory : drawCategories) {
    if (draws[drawCategory].empty()) {
      continue;
    }

    cout << ToString(drawCategory) << ": ";
    const auto& hands = draws[drawCategory];

    for (auto hand : hands) {
      cout << hand.second << (hand == *(hands.rbegin()) ? "\n" : " ");
    }
  }
}

GameType PokerGame::getGameType() const {
  return gameType_;
}
const vector<Range>& PokerGame::getRanges() const {
  return ranges_;
}

const CardSet& PokerGame::getBoard() const {
  return board_;
}

const CardSet& PokerGame::getDead() const {
  return dead_;
}

void PokerGame::setGameType(GameType gameType) {
  gameType_ = std::move(gameType);
}

void PokerGame::setRanges(vector<Range> ranges) {
  ranges_ = std::move(ranges);
}

void PokerGame::setBoard(CardSet board) {
  board_ = std::move(board);
}

void PokerGame::setDead(CardSet dead) {
  dead_ = std::move(dead);
}
