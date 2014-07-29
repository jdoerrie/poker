#pragma once

#include "Evaluator.h"

#include <string>
#include <utility>
#include <vector>

class HoldemEvaluator : public Evaluator {
 public:
    static std::vector<std::pair<std::string, double>> evalHands(
      const std::vector<std::string>& players,
      const std::string& board = "",
      const std::string& dead = "");

    static std::vector<std::pair<std::string, double>> bestHandForBoardAndRange(
      const std::string& board, const std::vector<std::string>& range);
};
