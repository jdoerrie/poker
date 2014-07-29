#include "HoldemEvaluator.h"

#include <iostream>

using namespace std;

vector<pair<string, double>> HoldemEvaluator::evalHands(
        const vector<string>& players,
        const string& board,
        const string& dead) {
  size_t numPlayers = players.size();

  vector<vector<int>> handsIds;
  handsIds.reserve(numPlayers);

  for (auto player: players) {
    handsIds.push_back(parseCards(player));
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

  array<int, 5> starts = {1, 1, 1, 1, 1};
  array<int, 5> ends = {52, 52, 52, 52, 52};

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

    int r0 = handRanks[h0 + 53];

    for (int h1 = starts[1]; h1 <= ends[1]; ++h1) {
      if (boardIds.size() <= 1 && isUsed[h1]) {
        continue;
      }

      if (boardIds.size() <= 1) {
        isUsed[h1] = true;
        starts[2] = h1 + 1;
      }

      int r1 = handRanks[h1 + r0];

      for (int h2 = starts[2]; h2 <= ends[2]; ++h2) {
        if (boardIds.size() <= 2 && isUsed[h2]) {
          continue;
        }

        if (boardIds.size() <= 2) {
          isUsed[h2] = true;
          starts[3] = h2 + 1;
        }

        int r2 = handRanks[h2 + r1];

        for (int h3 = starts[3]; h3 <= ends[3]; ++h3) {
          if (boardIds.size() <= 3 && isUsed[h3]) {
            continue;
          }

          if (boardIds.size() <= 3) {
            isUsed[h3] = true;
            starts[4] = h3 + 1;
          }

          int r3 = handRanks[h3 + r2];

          for (int h4 = starts[4]; h4 <= ends[4]; ++h4) {
            if (boardIds.size() <= 4 && isUsed[h4]) {
              continue;
            }

            int r4 = handRanks[h4 + r3];

            int maxRank = -1;
            int numBestPlayers = 0;

            vector<int> ranks(numPlayers);
            for (auto i = 0; i < numPlayers; ++i) {
              ranks[i] = r4;
              for (int card: handsIds[i]) {
                ranks[i] = handRanks[ranks[i] + card];
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

// static vector<pair<string, double>> bestHandForBoardAndRange(
//   const string& board, const vector<string>& range) {

// }

