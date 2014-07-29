#include "OmahaEvaluator.h"

#include <iostream>

using namespace std;

vector<pair<string, double>>  OmahaEvaluator::evalHands(
        const vector<string>& players,
        const string& board,
        const string& dead) {
  size_t numPlayers = players.size();

  vector<vector<int>> handsIds;
  for (auto player: players) {
    handsIds.emplace_back(parseCards(player));
  }

  vector<int> boardIds = parseCards(board);
  vector<int> deadIds = parseCards(dead);

  vector<int> isUsed(53);
  for (auto handIds: handsIds) {
    for (auto cardId: handIds) {
      isUsed[cardId] = true;
    }
  }

  for (auto boardId: boardIds) {
    isUsed[boardId] = true;
  }

  for (auto deadId: deadIds) {
    isUsed[deadId] = true;
  }

  vector<double> wonGames(players.size());
  int64_t numGames = 0;

  std::array<int, 5> starts = {1, 1, 1, 1, 1};
  std::array<int, 5> ends = {52, 52, 52, 52, 52};

  for (size_t i = 0; i < boardIds.size(); ++i) {
    starts[i] = ends[i] = boardIds[i];
  }

  for (int h0 = starts[0]; h0 <= ends[0]; ++h0) {
    if (boardIds.size() <= 0 && isUsed[h0]) {
      continue;
    }

    if (boardIds.size() <= 0) {
      isUsed[h0] = true;
      starts[1] = h0 + 1;
    }

    for (int h1 = starts[1]; h1 <= ends[1]; ++h1) {
      if (boardIds.size() <= 1 && isUsed[h1]) {
        continue;
      }

      if (boardIds.size() <= 1) {
        isUsed[h1] = true;
        starts[2] = h1 + 1;
      }

      for (int h2 = starts[2]; h2 <= ends[2]; ++h2) {
        if (boardIds.size() <= 2 && isUsed[h2]) {
          continue;
        }

        if (boardIds.size() <= 2) {
          isUsed[h2] = true;
          starts[3] = h2 + 1;
        }

        for (int h3 = starts[3]; h3 <= ends[3]; ++h3) {
          if (boardIds.size() <= 3 && isUsed[h3]) {
            continue;
          }

          if (boardIds.size() <= 3) {
            isUsed[h3] = true;
            starts[4] = h3 + 1;
          }

          for (int h4 = starts[4]; h4 <= ends[4]; ++h4) {
            if (boardIds.size() <= 4 && isUsed[h4]) {
              continue;
            }

            size_t numPlayers = handsIds.size();

            int maxRank = -1;
            int numBestPlayers = 0;

            vector<int> ranks(numPlayers, -1);
            array<int, 5> currentBoard = {h0, h1, h2, h3, h4};

            for (size_t i = 0; i < numPlayers; ++i) {
              size_t numCards = handsIds[i].size();

              for (int b0 = 0; b0 < 5; ++b0) {
                for (int b1 = b0 + 1; b1 < 5; ++b1) {
                  for (int b2 = b1 + 1; b2 < 5; ++b2) {
                    for (int p0 = 0; p0 < numCards; ++p0) {
                      for (int p1 = p0 + 1; p1 < numCards; ++p1) {
                        array<int, 5> currentHand = {
                          currentBoard[b0], currentBoard[b1], currentBoard[b2],
                          handsIds[i][p0], handsIds[i][p1]
                        };

                        int currentRank = 53;
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

            for (size_t i = 0; i < numPlayers; ++i) {
              if (ranks[i] == maxRank) {
                wonGames[i] += 1.0 / numBestPlayers;
              }
            }

            ++numGames;
          }

          if (boardIds.size() <= 3) {
            isUsed[h3] = false;
          }
        }

        if (boardIds.size() <= 2) {
          isUsed[h2] = false;
        }
      }

      if (boardIds.size() <= 1) {
        isUsed[h1] = false;
      }
    }

    if (boardIds.size() <= 0) {
      isUsed[h0] = false;
    }
  }

  vector<pair<string, double>> equities;
  equities.reserve(numPlayers);
  for (size_t i = 0; i < numPlayers; ++i) {
    equities.push_back({players[i], wonGames[i] / numGames});
  }

  return equities;
}
