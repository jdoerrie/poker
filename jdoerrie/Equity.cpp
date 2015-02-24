#include "Equity.h"

using namespace std;

Equity::Equity() : numGames_(0) {}

void Equity::addWin() {
  addGame(1);
}

void Equity::addTie(size_t numPlayers) {
  addGame(numPlayers);
}

void Equity::addLose() {
  addGame(0);
}

int Equity::getNumGames() const {
  return numGames_;
}

const vector<int>& Equity::getGameResults() const {
  return gameResults_;
}

double Equity::toDouble() const {
  if (numGames_ == 0) {
    return 0.0;
  }

  double result = 0.0;
  for (size_t i = 1; i < gameResults_.size(); ++i) {
    result += static_cast<double>(gameResults_[i]) / i;
  }

  return result / numGames_;
}

void Equity::merge(const Equity& other) {
  if (gameResults_.size() < other.gameResults_.size()) {
    gameResults_.resize(other.gameResults_.size());
  }

  for (size_t i = 0; i < other.gameResults_.size(); ++i) {
    gameResults_[i] += other.gameResults_[i];
  }

  numGames_ += other.numGames_;
}


bool Equity::operator<(const Equity& other) const {
  return toDouble() < other.toDouble();
}

void Equity::addGame(size_t result) {
  if (gameResults_.size() < result + 1) {
    gameResults_.resize(result + 1);
  }

  ++gameResults_[result];
  ++numGames_;
}

ostream& operator<<(ostream& out, const Equity& eq) {
  out << std::fixed << eq.toDouble();
  return out;
}

