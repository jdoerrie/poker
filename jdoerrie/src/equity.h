#pragma once

#include <ostream>
#include <string>
#include <vector>

class Equity {
 public:
  using ResultsType = std::vector<size_t>;

  Equity();
  explicit Equity(ResultsType results);

  void AddWin();
  void AddTie(size_t num_players = 2);
  void AddLoss();

  size_t wins() const;
  size_t ties(size_t num_players = 2) const;
  size_t losses() const;
  size_t num_ties() const;
  size_t num_games() const;

  double ToDouble() const;
  Equity& operator|=(const Equity& other);

 private:
  void AddResult(size_t result);

  ResultsType results_;
  size_t num_games_;
};

std::string colorEquity(double equity);
std::string colorEquity(const Equity& equity);
bool operator<(const Equity& lhs, const Equity& rhs);
std::ostream& operator<<(std::ostream& out, const Equity& equity);
