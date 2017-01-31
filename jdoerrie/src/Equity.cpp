#include "Equity.h"

#include <array>

using namespace std;

Equity::Equity() : gameResults_(3), numGames_(0) {}

void Equity::addWin() {
  addGame(1);
}

void Equity::addTie(size_t numPlayers) {
  addGame(numPlayers);
}

void Equity::addLose() {
  addGame(0);
}

size_t Equity::getNumGames() const {
  return numGames_;
}

const vector<size_t>& Equity::getGameResults() const {
  return gameResults_;
}

double Equity::ToDouble() const {
  if (numGames_ == 0) {
    return 0.0;
  }

  double result = 0.0;
  for (size_t i = 1; i < gameResults_.size(); ++i) {
    result += static_cast<double>(gameResults_[i]) / i;
  }

  return result / numGames_;
}

Equity& Equity::operator|=(const Equity& other) {
  if (gameResults_.size() < other.gameResults_.size()) {
    gameResults_.resize(other.gameResults_.size());
  }

  for (size_t i = 0; i < other.gameResults_.size(); ++i) {
    gameResults_[i] += other.gameResults_[i];
  }

  numGames_ += other.numGames_;
  return *this;
}

void Equity::addGame(size_t result) {
  if (gameResults_.size() < result + 1) {
    gameResults_.resize(result + 1);
  }

  ++gameResults_[result];
  ++numGames_;
}

bool operator<(const Equity& lhs, const Equity& rhs) {
  return lhs.ToDouble() < rhs.ToDouble();
}

ostream& operator<<(ostream& out, const Equity& equity) {
  out << std::fixed << equity.ToDouble();
  return out;
}

string colorEquity(double equity) {
  static const array<string, 11> gradient = {
      {"\x1B[38;5;196m", "\x1B[38;5;202m", "\x1B[38;5;208m", "\x1B[38;5;214m",
       "\x1B[38;5;220m", "\x1B[38;5;226m", "\x1B[38;5;190m", "\x1B[38;5;154m",
       "\x1B[38;5;118m", "\x1B[38;5;82m", "\x1B[38;5;46m"}};

  size_t idx = static_cast<size_t>(equity * gradient.size());
  if (idx == gradient.size()) {
    --idx;
  }

  return gradient[idx] + to_string(equity) + "\x1B[0m";
}

string colorEquity(const Equity& equity) {
  return colorEquity(equity.ToDouble());
}
