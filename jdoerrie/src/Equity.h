#pragma once

#include <ostream>
#include <string>
#include <vector>

class Equity {
 public:
  Equity();

  void addWin();
  void addTie(size_t numPlayers = 2);
  void addLose();

  size_t getNumGames() const;
  const std::vector<size_t>& getGameResults() const;

  double ToDouble() const;
  void merge(const Equity& other);

 private:
  void addGame(size_t result);

  std::vector<size_t> gameResults_;
  size_t numGames_;
};

std::string colorEquity(double equity);
std::string colorEquity(const Equity& equity);
bool operator<(const Equity& lhs, const Equity& rhs);
std::ostream& operator<<(std::ostream& out, const Equity& equity);
