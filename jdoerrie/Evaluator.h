#pragma once

#include <array>
#include <string>
#include <vector>

/**
 * Card to integer conversions:
 * 2c =  1  2d =  2  2h =  3  2s =  4
 * 3c =  5  3d =  6  3h =  7  3s =  8
 * 4c =  9  4d = 10  4h = 11  4s = 12
 * 5c = 13  5d = 14  5h = 15  5s = 16
 * 6c = 17  6d = 18  6h = 19  6s = 20
 * 7c = 21  7d = 22  7h = 23  7s = 24
 * 8c = 25  8d = 26  8h = 27  8s = 28
 * 9c = 29  9d = 30  9h = 31  9s = 32
 * Tc = 33  Td = 34  Th = 35  Ts = 36
 * Jc = 37  Jd = 38  Jh = 39  Js = 40
 * Qc = 41  Qd = 42  Qh = 43  Qs = 44
 * Kc = 45  Kd = 46  Kh = 47  Ks = 48
 * Ac = 49  Ad = 50  Ah = 51  As = 52
 */

enum class GameType;

class CardCollection;
class Equity;
class Range;

class Evaluator {
 public:
  static std::array<int, 32487834> handRanks;

  /**
   * Initializes the Evaluator class with the HandRank file.
   * @param  fileName Filename of HandRanks.dat
   * @return          boolean indicating whether the file could be read
   */
  static bool initialize(const std::string& fileName);

  /**
   * Given a hand as a vector<int> returns the rank of the hand.
   * Supports 5, 6 and 7 cards. Returns 0 on error.
   * @param  hand CardCollection vector
   * @return      Hank Rank
   */
  static int getHandRank(const CardCollection& hand);

  static std::vector<Equity> evalRanges(
      GameType gameType,
      const std::vector<Range>& ranges,
      const CardCollection& board,
      const CardCollection& dead
  );

  static std::vector<Equity> evalHands(
      GameType gameType,
      const std::vector<CardCollection>& hands,
      const CardCollection& board,
      const CardCollection& dead
  );

  static std::vector<Equity> evalHoldemRanges(
      const std::vector<Range>& ranges,
      const CardCollection& board,
      const CardCollection& dead
  );

 private:
  static std::vector<Equity> evalRangesHelper(
      GameType gameType,
      const std::vector<Range>& ranges,
      const CardCollection& boardCards,
      const CardCollection& deadCards,
      std::vector<CardCollection> currHands = {}
  );

  static std::vector<Equity> evalHoldemHands(
      const std::vector<CardCollection>& hands,
      const CardCollection& board,
      const CardCollection& dead
  );

  static std::vector<Equity> evalOmahaHands(
      const std::vector<CardCollection>& hands,
      const CardCollection& board,
      const CardCollection& dead
  );



  static void evalHoldemRangesHelper(
    std::vector<Equity>& equities, int numCards, size_t currId, int currRank,
    size_t usedIds, const std::vector<Range>& ranges
  );

  static void rankHoldemRanges(
    std::vector<Equity>& equities, size_t numPlayers, int boardRank,
    const std::vector<Range>& ranges, std::vector<size_t>& indices,
    size_t usedIds
  );
};
