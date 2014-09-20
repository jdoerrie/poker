#include "Evaluator.h"

#include "Card.h"
#include "Equity.h"
#include "Hand.h"
#include "Range.h"

#include <cstdio>
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>

using namespace std;

array<int, 32487834> Evaluator::handRanks;

bool Evaluator::initialize(const string& fileName) {
  FILE *fin = fopen(fileName.c_str(), "rb");
  if (fin == nullptr) {
    return false;
  }

  fread(handRanks.data(), sizeof(int), handRanks.size(), fin);
  fclose(fin);
  return true;
}

int Evaluator::getHandRank(const Hand& hand) {
  if (hand.getCards().size() < 5 || hand.getCards().size() > 7) {
    return 0;
  }

  int rank = Card::MAX_ID + 1;
  for (auto card: hand.getCards()) {
    rank = handRanks[rank + card.getId()];
  }

  if (hand.getCards().size() != 7) {
    rank = handRanks[rank];
  }

  return rank;
}

vector<Equity> Evaluator::evalRanges(
    GameType gameType,
    const vector<Range>& ranges,
    const Hand& board,
    const Hand& dead
) {
  return evalRangesHelper(gameType, ranges, board, dead);
}

vector<Equity> Evaluator::evalHands(
    GameType gameType,
    const vector<Hand>& hands,
    const Hand& board,
    const Hand& dead
) {
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

vector<Equity> Evaluator::evalRangesHelper(
    GameType gameType,
    const vector<Range>& ranges,
    const Hand& board,
    const Hand& dead,
    vector<Hand> currHands
) {
  if (currHands.size() == ranges.size()) {
    return evalHands(gameType, currHands, board, dead);
  }

  vector<Equity> equities(ranges.size());
  for (const auto& Hand: ranges[currHands.size()].getHands()) {
    currHands.push_back(Hand);
    auto result = evalRangesHelper(gameType, ranges, board, dead, currHands);
    currHands.pop_back();

    for (int i = 0; i < ranges.size(); ++i) {
      equities[i].merge(result[i]);
    }
  }

  return equities;
}


vector<Equity> Evaluator::evalHoldemHands(
    const vector<Hand>& hands,
    const Hand& board,
    const Hand& dead
) {
  size_t numPlayers = hands.size();
  vector<Equity> equities(numPlayers);

  vector<bool> isUsed(Card::MAX_ID + 1);
  bool isValidGame = board.isValid() && dead.isValid();

  for (auto hand: hands) {
    isValidGame &= hand.isValid(2);
    for (auto card: hand.getCards()) {
      isValidGame &= !isUsed[card.getId()];
      isUsed[card.getId()] = true;
    }
  }

  for (auto card: board.getCards()) {
    isValidGame &= !isUsed[card.getId()];
    isUsed[card.getId()] = true;
  }

  for (auto card: dead.getCards()) {
    isValidGame &= !isUsed[card.getId()];
    isUsed[card.getId()] = true;
  }

  if (!isValidGame) {
    return equities;
  }

  vector<int> ranks(numPlayers);

  vector<int> starts(5, 1);
  vector<int> ends(5, Card::MAX_ID);

  for (size_t i = 0; i < board.getCards().size(); ++i) {
    starts[i] = ends[i] = board.getCards()[i].getId();
  }

  for (int h0 = starts[0]; h0 <= ends[0]; ++h0) {
    if (board.getCards().empty() && isUsed[h0]) {
      continue;
    }

    if (board.getCards().empty()) {
      isUsed[h0] = true;
      starts[1] = h0 + 1;
    }

    int r0 = handRanks[h0 + Card::MAX_ID + 1];

    for (int h1 = starts[1]; h1 <= ends[1]; ++h1) {
      if (board.getCards().size() <= 1 && isUsed[h1]) {
        continue;
      }

      if (board.getCards().size() <= 1) {
        isUsed[h1] = true;
        starts[2] = h1 + 1;
      }

      int r1 = handRanks[h1 + r0];
      for (int h2 = starts[2]; h2 <= ends[2]; ++h2) {
        if (board.getCards().size() <= 2 && isUsed[h2]) {
          continue;
        }

        if (board.getCards().size() <= 2) {
          isUsed[h2] = true;
          starts[3] = h2 + 1;
        }

        int r2 = handRanks[h2 + r1];
        for (int h3 = starts[3]; h3 <= ends[3]; ++h3) {
          if (board.getCards().size() <= 3 && isUsed[h3]) {
            continue;
          }

          if (board.getCards().size() <= 3) {
            isUsed[h3] = true;
            starts[4] = h3 + 1;
          }

          int r3 = handRanks[h3 + r2];
          for (int h4 = starts[4]; h4 <= ends[4]; ++h4) {
            if (board.getCards().size() <= 4 && isUsed[h4]) {
              continue;
            }

            int r4 = handRanks[h4 + r3];
            int maxRank = -1;
            int numBestPlayers = 0;

            for (auto i = 0; i < numPlayers; ++i) {
              ranks[i] = r4;
              for (auto card: hands[i].getCards()) {
                ranks[i] = handRanks[ranks[i] + card.getId()];
              }

              if (maxRank == ranks[i]) {
                ++numBestPlayers;
              } else if (maxRank < ranks[i]) {
                maxRank = ranks[i];
                numBestPlayers = 1;
              }
            }

            for (auto i = 0; i < numPlayers; ++i) {
              if (ranks[i] == maxRank) {
                equities[i].addTie(numBestPlayers);
              } else {
                equities[i].addLose();
              }
            }
          }

          if (board.getCards().size() <= 3) {
            isUsed[h3] = false;
          }
        }

        if (board.getCards().size() <= 2) {
          isUsed[h2] = false;
        }
      }

      if (board.getCards().size() <= 1) {
        isUsed[h1] = false;
      }
    }

    if (board.getCards().empty()) {
      isUsed[h0] = false;
    }
  }

  return equities;
}

vector<Equity> Evaluator::evalOmahaHands(
    const vector<Hand>& hands,
    const Hand& board,
    const Hand& dead
) {
  size_t numPlayers = hands.size();
  vector<Equity> equities(numPlayers);

  vector<bool> isUsed(Card::MAX_ID + 1);
  bool isValidGame = board.isValid() && dead.isValid();

  for (auto hand: hands) {
    isValidGame &= hand.isValid(2);
    for (auto card: hand.getCards()) {
      isValidGame &= !isUsed[card.getId()];
      isUsed[card.getId()] = true;
    }
  }

  for (auto card: board.getCards()) {
    isValidGame &= !isUsed[card.getId()];
    isUsed[card.getId()] = true;
  }

  for (auto card: dead.getCards()) {
    isValidGame &= !isUsed[card.getId()];
    isUsed[card.getId()] = true;
  }

  if (!isValidGame) {
    return equities;
  }

  vector<int> ranks(numPlayers);

  vector<int> starts(5, 1);
  vector<int> ends(5, Card::MAX_ID);

  for (size_t i = 0; i < board.getCards().size(); ++i) {
    starts[i] = ends[i] = board.getCards()[i].getId();
  }

  for (int h0 = starts[0]; h0 <= ends[0]; ++h0) {
    if (board.getCards().size() <= 0 && isUsed[h0]) {
      continue;
    }

    if (board.getCards().size() <= 0) {
      isUsed[h0] = true;
      starts[1] = h0 + 1;
    }

    for (int h1 = starts[1]; h1 <= ends[1]; ++h1) {
      if (board.getCards().size() <= 1 && isUsed[h1]) {
        continue;
      }

      if (board.getCards().size() <= 1) {
        isUsed[h1] = true;
        starts[2] = h1 + 1;
      }

      for (int h2 = starts[2]; h2 <= ends[2]; ++h2) {
        if (board.getCards().size() <= 2 && isUsed[h2]) {
          continue;
        }

        if (board.getCards().size() <= 2) {
          isUsed[h2] = true;
          starts[3] = h2 + 1;
        }

        for (int h3 = starts[3]; h3 <= ends[3]; ++h3) {
          if (board.getCards().size() <= 3 && isUsed[h3]) {
            continue;
          }

          if (board.getCards().size() <= 3) {
            isUsed[h3] = true;
            starts[4] = h3 + 1;
          }

          for (int h4 = starts[4]; h4 <= ends[4]; ++h4) {
            if (board.getCards().size() <= 4 && isUsed[h4]) {
              continue;
            }

            int maxRank = -1;
            int numBestPlayers = 0;

            array<int, 5> currentBoard = {h0, h1, h2, h3, h4};

            for (size_t i = 0; i < numPlayers; ++i) {
              size_t numCards = hands[i].getCards().size();

              for (int b0 = 0; b0 < 5; ++b0) {
                for (int b1 = b0 + 1; b1 < 5; ++b1) {
                  for (int b2 = b1 + 1; b2 < 5; ++b2) {
                    for (int p0 = 0; p0 < numCards; ++p0) {
                      for (int p1 = p0 + 1; p1 < numCards; ++p1) {
                        array<int, 5> currentHand = {
                          currentBoard[b0],
                          currentBoard[b1],
                          currentBoard[b2],
                          static_cast<int>(hands[i].getCards()[p0].getId()),
                          static_cast<int>(hands[i].getCards()[p1].getId())
                        };

                        int currentRank = Card::MAX_ID + 1;
                        for (int hand: currentHand) {
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

            for (auto i = 0; i < numPlayers; ++i) {
              if (ranks[i] == maxRank) {
                equities[i].addTie(numBestPlayers);
              } else {
                equities[i].addLose();
              }
            }
          }

          if (board.getCards().size() <= 3) {
            isUsed[h3] = false;
          }
        }

        if (board.getCards().size() <= 2) {
          isUsed[h2] = false;
        }
      }

      if (board.getCards().size() <= 1) {
        isUsed[h1] = false;
      }
    }

    if (board.getCards().size() <= 0) {
      isUsed[h0] = false;
    }
  }

  return equities;
}
