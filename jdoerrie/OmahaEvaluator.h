#pragma once

#include "Evaluator.h"

class OmahaEvaluator : public Evaluator {
 public:
    static std::vector<std::pair<std::string, double>>  evalHands(
      const std::vector<std::string>& players,
      const std::string& board = "",
      const std::string& dead = "");
};
