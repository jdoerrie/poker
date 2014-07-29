#pragma once

#include <array>
#include <string>
#include <unordered_map>
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

class Evaluator {
 protected:
  static const std::string ranks;
  static const std::string suits;

  static const std::unordered_map<char, int> rankMap;
  static const std::unordered_map<char, int> suitMap;

  static void evalHandsHelper(const std::vector<std::vector<int>> handsIds,
                              int currRank, std::vector<int>& isUsed,
                              std::vector<double>& wonGames, int64_t& numGames,
                              int streetsLeft, int prevId);

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
   * @param  hand Hand vector
   * @return      Hank Rank
   */
  static int lookUpHandRank(const std::vector<int>& hand);

  static std::vector<int> parseCards(const std::string& hand);
  static std::string toString(const std::vector<int>& hand);

  static void printEquities(
    const std::vector<std::pair<std::string, double>>& equities);

  static std::vector<std::pair<std::string, double>> bestCardForHero(
    const std::vector<std::string>& heroRange,
    const std::vector<std::string>& villainRange,
    const std::string& board,
    const std::string& dead,
    const std::string& game);
};
