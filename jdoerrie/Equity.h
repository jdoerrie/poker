#pragma once

#include <ostream>
#include <vector>

class Equity {
 public:
  Equity();

  void addWin();
  void addTie(size_t numPlayers);
  void addLose();

  size_t getNumGames() const;
  const std::vector<size_t>& getGameResults() const;

  double toDouble() const;
  void merge(const Equity& other);

  bool operator<(const Equity& other) const;
  friend std::ostream& operator<<(std::ostream& out, const Equity& eq);

 private:
  void addGame(size_t result);

  std::vector<size_t> gameResults_;
  size_t numGames_;
};
