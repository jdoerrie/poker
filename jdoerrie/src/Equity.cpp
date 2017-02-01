#include "Equity.h"

#include <array>
#include <cassert>
#include <numeric>

Equity::Equity() : results_{ResultsType(3)}, num_games_{0} {}

Equity::Equity(ResultsType results)
    : results_{std::move(results)},
      num_games_{
          std::accumulate(std::begin(results_), std::end(results_), 0u)} {}

void Equity::AddWin() {
  AddResult(1);
}

void Equity::AddTie(size_t num_players) {
  AddResult(num_players);
}

void Equity::AddLoss() {
  AddResult(0);
}

size_t Equity::wins() const {
  return results_[1];
}

size_t Equity::ties(size_t num_players) const {
  assert(num_players < results_.size());
  return results_[num_players];
}

size_t Equity::losses() const {
  return results_[0];
}

size_t Equity::num_ties() const {
  return results_.size() - 1;
}

size_t Equity::num_games() const {
  return num_games_;
}

double Equity::ToDouble() const {
  if (num_games_ == 0) {
    return 0.0;
  }

  double result = 0.0;
  for (size_t i = 1; i < results_.size(); ++i) {
    result += static_cast<double>(results_[i]) / i;
  }

  return result / num_games_;
}

Equity& Equity::operator|=(const Equity& other) {
  if (results_.size() < other.results_.size()) {
    results_.resize(other.results_.size());
  }

  for (size_t i = 0; i < other.results_.size(); ++i) {
    results_[i] += other.results_[i];
  }

  num_games_ += other.num_games_;
  return *this;
}

void Equity::AddResult(size_t result) {
  if (results_.size() < result + 1) {
    results_.resize(result + 1);
  }

  ++results_[result];
  ++num_games_;
}

bool operator<(const Equity& lhs, const Equity& rhs) {
  return lhs.ToDouble() < rhs.ToDouble();
}

std::ostream& operator<<(std::ostream& out, const Equity& equity) {
  return out << std::fixed << equity.ToDouble();
}

std::string colorEquity(double equity) {
  static const std::array<std::string, 11> gradient = {
      {"\x1B[38;5;196m", "\x1B[38;5;202m", "\x1B[38;5;208m", "\x1B[38;5;214m",
       "\x1B[38;5;220m", "\x1B[38;5;226m", "\x1B[38;5;190m", "\x1B[38;5;154m",
       "\x1B[38;5;118m", "\x1B[38;5;82m", "\x1B[38;5;46m"}};

  size_t idx = static_cast<size_t>(equity * gradient.size());
  if (idx == gradient.size()) {
    --idx;
  }

  return gradient[idx] + std::to_string(equity) + "\x1B[0m";
}

std::string colorEquity(const Equity& equity) {
  return colorEquity(equity.ToDouble());
}
