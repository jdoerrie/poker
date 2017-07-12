#include "evaluator.h"

#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "card.h"
#include "card_set.h"
#include "equity.h"
#include "game_type.h"
#include "range.h"

using namespace std;

array<int, 32487834> Evaluator::handRanks;

bool Evaluator::initialize(const string& fileName) {
  FILE* fin = fopen(fileName.c_str(), "rb");
  if (fin == nullptr) {
    return false;
  }

  fread(handRanks.data(), sizeof(int), handRanks.size(), fin);
  fclose(fin);
  return true;
}

int Evaluator::getHandRank(const CardSet& hand) {
  if (hand.ToCards().size() < 5 || hand.ToCards().size() > 7) {
    return 0;
  }

  int rank = Card::MAX_ID + 1;
  for (auto card : hand.ToCards()) {
    rank = handRanks[rank + card.id()];
  }

  if (hand.ToCards().size() != 7) {
    rank = handRanks[rank];
  }

  return rank;
}

vector<Equity> Evaluator::evalRanges(GameType gameType,
                                     const vector<Range>& ranges,
                                     const CardSet& board,
                                     const CardSet& dead) {
  if (gameType == GameType::HOLDEM) {
    return evalHoldemRanges(ranges, board, dead);
  }

  return evalRangesHelper(gameType, ranges, board, dead);
}

vector<Equity> Evaluator::evalHands(GameType gameType,
                                    const vector<CardSet>& hands,
                                    const CardSet& board,
                                    const CardSet& dead) {
  switch (gameType) {
    case GameType::HOLDEM:
      return Evaluator::evalHoldemHands(hands, board, dead);

    case GameType::OMAHA:
    case GameType::OMAHA5:
      return Evaluator::evalOmahaHands(hands, board, dead);

    default:
      return {};
  }
}

vector<Equity> Evaluator::evalRangesHelper(GameType gameType,
                                           const vector<Range>& ranges,
                                           const CardSet& board,
                                           const CardSet& dead,
                                           vector<CardSet> currHands) {
  if (currHands.size() == ranges.size()) {
    return evalHands(gameType, currHands, board, dead);
  }

  vector<Equity> equities(ranges.size());
  for (const auto& CardSet : ranges[currHands.size()].getHands()) {
    currHands.push_back(CardSet);
    auto result = evalRangesHelper(gameType, ranges, board, dead, currHands);
    currHands.pop_back();

    for (size_t i = 0; i < ranges.size(); ++i) {
      equities[i] |= result[i];
    }
  }

  return equities;
}

vector<Equity> Evaluator::evalHoldemHands(const vector<CardSet>& hands,
                                          const CardSet& board,
                                          const CardSet& dead) {
  size_t numPlayers = hands.size();
  vector<Equity> equities(numPlayers);

  vector<bool> isUsed(Card::MAX_ID + 1);
  bool IsValidGame = board.IsValid() && dead.IsValid();

  for (auto hand : hands) {
    IsValidGame &= hand.IsValid(2);
    for (auto card : hand.ToCards()) {
      IsValidGame &= !isUsed[card.id()];
      isUsed[card.id()] = true;
    }
  }

  for (auto card : board.ToCards()) {
    IsValidGame &= !isUsed[card.id()];
    isUsed[card.id()] = true;
  }

  for (auto card : dead.ToCards()) {
    IsValidGame &= !isUsed[card.id()];
    isUsed[card.id()] = true;
  }

  if (!IsValidGame) {
    return equities;
  }

  vector<int> ranks(numPlayers);

  vector<int> starts(5, 1);
  vector<int> ends(5, Card::MAX_ID);

  for (size_t i = 0; i < board.ToCards().size(); ++i) {
    starts[i] = ends[i] = board.ToCards()[i].id();
  }

  for (int h0 = starts[0]; h0 <= ends[0]; ++h0) {
    if (board.ToCards().empty() && isUsed[h0]) {
      continue;
    }

    if (board.ToCards().empty()) {
      isUsed[h0] = true;
      starts[1] = h0 + 1;
    }

    int r0 = handRanks[h0 + Card::MAX_ID + 1];

    for (int h1 = starts[1]; h1 <= ends[1]; ++h1) {
      if (board.ToCards().size() <= 1 && isUsed[h1]) {
        continue;
      }

      if (board.ToCards().size() <= 1) {
        isUsed[h1] = true;
        starts[2] = h1 + 1;
      }

      int r1 = handRanks[h1 + r0];
      for (int h2 = starts[2]; h2 <= ends[2]; ++h2) {
        if (board.ToCards().size() <= 2 && isUsed[h2]) {
          continue;
        }

        if (board.ToCards().size() <= 2) {
          isUsed[h2] = true;
          starts[3] = h2 + 1;
        }

        int r2 = handRanks[h2 + r1];
        for (int h3 = starts[3]; h3 <= ends[3]; ++h3) {
          if (board.ToCards().size() <= 3 && isUsed[h3]) {
            continue;
          }

          if (board.ToCards().size() <= 3) {
            isUsed[h3] = true;
            starts[4] = h3 + 1;
          }

          int r3 = handRanks[h3 + r2];
          for (int h4 = starts[4]; h4 <= ends[4]; ++h4) {
            if (board.ToCards().size() <= 4 && isUsed[h4]) {
              continue;
            }

            int r4 = handRanks[h4 + r3];
            int maxRank = -1;
            int numBestPlayers = 0;

            for (size_t i = 0; i < numPlayers; ++i) {
              ranks[i] = r4;
              for (auto card : hands[i].ToCards()) {
                ranks[i] = handRanks[ranks[i] + card.id()];
              }

              if (maxRank == ranks[i]) {
                ++numBestPlayers;
              } else if (maxRank < ranks[i]) {
                maxRank = ranks[i];
                numBestPlayers = 1;
              }
            }

            for (size_t i = 0; i < numPlayers; ++i) {
              if (ranks[i] == maxRank) {
                equities[i].AddTie(numBestPlayers);
              } else {
                equities[i].AddLoss();
              }
            }
          }

          if (board.ToCards().size() <= 3) {
            isUsed[h3] = false;
          }
        }

        if (board.ToCards().size() <= 2) {
          isUsed[h2] = false;
        }
      }

      if (board.ToCards().size() <= 1) {
        isUsed[h1] = false;
      }
    }

    if (board.ToCards().empty()) {
      isUsed[h0] = false;
    }
  }

  return equities;
}

vector<Equity> Evaluator::evalOmahaHands(const vector<CardSet>& hands,
                                         const CardSet& board,
                                         const CardSet& dead) {
  size_t numPlayers = hands.size();
  vector<Equity> equities(numPlayers);

  vector<bool> isUsed(Card::MAX_ID + 1);
  bool IsValidGame = board.IsValid() && dead.IsValid();

  for (auto hand : hands) {
    IsValidGame &= hand.IsValid(2);
    for (auto card : hand.ToCards()) {
      IsValidGame &= !isUsed[card.id()];
      isUsed[card.id()] = true;
    }
  }

  for (auto card : board.ToCards()) {
    IsValidGame &= !isUsed[card.id()];
    isUsed[card.id()] = true;
  }

  for (auto card : dead.ToCards()) {
    IsValidGame &= !isUsed[card.id()];
    isUsed[card.id()] = true;
  }

  if (!IsValidGame) {
    return equities;
  }

  vector<int> ranks(numPlayers);

  vector<int> starts(5, 1);
  vector<int> ends(5, Card::MAX_ID);

  for (size_t i = 0; i < board.ToCards().size(); ++i) {
    starts[i] = ends[i] = board.ToCards()[i].id();
  }

  for (int h0 = starts[0]; h0 <= ends[0]; ++h0) {
    if (board.ToCards().size() <= 0 && isUsed[h0]) {
      continue;
    }

    if (board.ToCards().size() <= 0) {
      isUsed[h0] = true;
      starts[1] = h0 + 1;
    }

    for (int h1 = starts[1]; h1 <= ends[1]; ++h1) {
      if (board.ToCards().size() <= 1 && isUsed[h1]) {
        continue;
      }

      if (board.ToCards().size() <= 1) {
        isUsed[h1] = true;
        starts[2] = h1 + 1;
      }

      for (int h2 = starts[2]; h2 <= ends[2]; ++h2) {
        if (board.ToCards().size() <= 2 && isUsed[h2]) {
          continue;
        }

        if (board.ToCards().size() <= 2) {
          isUsed[h2] = true;
          starts[3] = h2 + 1;
        }

        for (int h3 = starts[3]; h3 <= ends[3]; ++h3) {
          if (board.ToCards().size() <= 3 && isUsed[h3]) {
            continue;
          }

          if (board.ToCards().size() <= 3) {
            isUsed[h3] = true;
            starts[4] = h3 + 1;
          }

          for (int h4 = starts[4]; h4 <= ends[4]; ++h4) {
            if (board.ToCards().size() <= 4 && isUsed[h4]) {
              continue;
            }

            int maxRank = -1;
            int numBestPlayers = 0;

            array<int, 5> currentBoard = {{h0, h1, h2, h3, h4}};

            for (size_t i = 0; i < numPlayers; ++i) {
              size_t numCards = hands[i].ToCards().size();

              for (size_t b0 = 0; b0 < 5; ++b0) {
                for (size_t b1 = b0 + 1; b1 < 5; ++b1) {
                  for (size_t b2 = b1 + 1; b2 < 5; ++b2) {
                    for (size_t p0 = 0; p0 < numCards; ++p0) {
                      for (size_t p1 = p0 + 1; p1 < numCards; ++p1) {
                        array<int, 5> currentHand = {
                            {currentBoard[b0], currentBoard[b1],
                             currentBoard[b2],
                             static_cast<int>(hands[i].ToCards()[p0].id()),
                             static_cast<int>(hands[i].ToCards()[p1].id())}};

                        int currentRank = Card::MAX_ID + 1;
                        for (size_t hand : currentHand) {
                          currentRank = handRanks[currentRank + hand];
                        }

                        currentRank = handRanks[currentRank];
                        ranks[i] = max(ranks[i], currentRank);
                      }
                    }
                  }
                }
              }

              if (maxRank == ranks[i]) {
                ++numBestPlayers;
              } else if (maxRank < ranks[i]) {
                maxRank = ranks[i];
                numBestPlayers = 1;
              }
            }

            for (size_t i = 0; i < numPlayers; ++i) {
              if (ranks[i] == maxRank) {
                equities[i].AddTie(numBestPlayers);
              } else {
                equities[i].AddLoss();
              }
            }
          }

          if (board.ToCards().size() <= 3) {
            isUsed[h3] = false;
          }
        }

        if (board.ToCards().size() <= 2) {
          isUsed[h2] = false;
        }
      }

      if (board.ToCards().size() <= 1) {
        isUsed[h1] = false;
      }
    }

    if (board.ToCards().size() <= 0) {
      isUsed[h0] = false;
    }
  }

  return equities;
}

vector<Equity> Evaluator::evalHoldemRanges(const vector<Range>& ranges,
                                           const CardSet& board,
                                           const CardSet& dead) {
  int numPlayers = ranges.size();

  vector<Equity> equities(numPlayers);
  if (board.id() & dead.id()) {
    return equities;
  }

  size_t usedIds = board.id() | dead.id();

  int handRank = Card::MAX_ID + 1;
  for (const auto& card : board.ToCards()) {
    handRank = handRanks[handRank + card.id()];
  }

  evalHoldemRangesHelper(equities, board.ToCards().size(), 1, handRank, usedIds,
                         ranges);

  return equities;
}

void Evaluator::evalHoldemRangesHelper(vector<Equity>& equities,
                                       int numCards,
                                       size_t currId,
                                       int currRank,
                                       size_t usedIds,
                                       const vector<Range>& ranges) {
  if (numCards == 5) {
    vector<size_t> indices(ranges.size());
    rankHoldemRanges(equities, 0, currRank, ranges, indices, usedIds);
    return;
  }

  while (currId <= Card::MAX_ID) {
    if (usedIds & (1LL << currId)) {
      ++currId;
      continue;
    }

    evalHoldemRangesHelper(equities, numCards + 1, currId + 1,
                           handRanks[currRank + currId],
                           usedIds | (1LL << currId), ranges);
    ++currId;
  }
}

void Evaluator::rankHoldemRanges(vector<Equity>& equities,
                                 size_t numPlayers,
                                 int boardRank,
                                 const vector<Range>& ranges,
                                 vector<size_t>& indices,
                                 size_t usedIds) {
  if (ranges.size() == 2) {  // optimized for heads-up
    for (const auto& hero : ranges[0].getHands()) {
      if (hero.id() & usedIds) {
        continue;
      }

      int heroRank = boardRank;
      heroRank = handRanks[heroRank + hero.ToCards()[0].id()];
      heroRank = handRanks[heroRank + hero.ToCards()[1].id()];

      for (const auto& vill : ranges[1].getHands()) {
        if (vill.id() & (usedIds | hero.id())) {
          continue;
        }

        int villRank = boardRank;
        villRank = handRanks[villRank + vill.ToCards()[0].id()];
        villRank = handRanks[villRank + vill.ToCards()[1].id()];

        if (heroRank > villRank) {
          equities[0].AddWin();
          equities[1].AddLoss();
        } else if (heroRank < villRank) {
          equities[0].AddLoss();
          equities[1].AddWin();
        } else {
          equities[0].AddTie();
          equities[1].AddTie();
        }
      }
    }

    return;
  }

  if (numPlayers == ranges.size()) {
    vector<int> ranks(numPlayers, boardRank);

    int maxRank = -1;
    int numBestPlayers = 0;
    for (size_t i = 0; i < numPlayers; ++i) {
      const auto& cards = ranges[i].getHands()[indices[i]].ToCards();
      ranks[i] = handRanks[ranks[i] + cards[0].id()];
      ranks[i] = handRanks[ranks[i] + cards[1].id()];
      if (maxRank == ranks[i]) {
        ++numBestPlayers;
      } else if (maxRank < ranks[i]) {
        maxRank = ranks[i];
        numBestPlayers = 1;
      }
    }

    for (size_t i = 0; i < numPlayers; ++i) {
      if (ranks[i] == maxRank) {
        equities[i].AddTie(numBestPlayers);
      } else {
        equities[i].AddLoss();
      }
    }

    return;
  }

  size_t& currIdx = indices[numPlayers];
  for (currIdx = 0; currIdx < ranges[numPlayers].getHands().size(); ++currIdx) {
    if (ranges[numPlayers].getHands()[currIdx].id() & usedIds) {
      continue;
    }

    rankHoldemRanges(equities, numPlayers + 1, boardRank, ranges, indices,
                     usedIds | ranges[numPlayers].getHands()[currIdx].id());
  }
}
