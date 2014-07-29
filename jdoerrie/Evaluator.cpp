#include "Evaluator.h"
#include "HoldemEvaluator.h"
#include "OmahaEvaluator.h"

#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

array<int, 32487834> Evaluator::handRanks;

const string Evaluator::ranks = "23456789TJQKA";
const string Evaluator::suits = "cdhs";

const unordered_map<char, int> Evaluator::rankMap = {
  {'2', 0}, {'3', 1}, {'4', 2}, {'5',  3}, {'6',  4}, {'7',  5}, {'8', 6},
  {'9', 7}, {'T', 8}, {'J', 9}, {'Q', 10}, {'K', 11}, {'A', 12}
};

const unordered_map<char, int> Evaluator::suitMap = {
  {'c', 0}, {'d', 1}, {'h', 2}, {'s', 3}
};

bool Evaluator::initialize(const string& fileName) {
  FILE *fin = fopen(fileName.c_str(), "rb");
  if (fin == NULL) {
    return false;
  }

  fread(handRanks.data(), sizeof(int), handRanks.size(), fin);
  fclose(fin);
  return true;
}

int Evaluator::lookUpHandRank(const vector<int>& hand) {
  if (hand.size() < 5 || hand.size() > 7) {
    return 0;
  }

  int rank = 53;
  for (auto card: hand) {
    rank = handRanks[rank + card];
  }

  if (hand.size() != 7) {
    rank = handRanks[rank];
  }

  return rank;
}

vector<int> Evaluator::parseCards(const string& hand) {
  vector<int> result;
  result.reserve(hand.size() / 2);
  for (int i = 0; i + 1 < hand.size(); i += 2) {
    int rank = rankMap.at(hand[i]);
    int suit = suitMap.at(hand[i + 1]);

    int cardId = rank * 4 + suit + 1;
    result.emplace_back(cardId);
  }

  return result;
}


string Evaluator::toString(const vector<int>& cards) {
  size_t numCards = cards.size();
  string result(numCards * 2, ' ');

  for (size_t i = 0; i < numCards; ++i) {
    char rank = ranks[(cards[i] - 1) / 4];
    char suit = suits[(cards[i] - 1) % 4];

    result[2 * i] = rank;
    result[2*i+1] = suit;
  }

  return result;
}

void Evaluator::printEquities(const vector<pair<string, double>>& equities) {
  for (size_t i = 0; i < equities.size(); ++i) {
    cout << equities[i].first << ": " << equities[i].second << endl;
  }
}

vector<pair<string, double>> Evaluator::bestCardForHero(
  const vector<string>& heroRange,
  const vector<string>& villainRange, const string& board,
  const string& dead, const string& game) {
  vector<pair<string, double>> result;

  bool isHoldem = (game == "holdem");

  vector<double> equities(53);
  vector<int> counts(53);

  vector<int> used(53);
  for (auto id: parseCards(board)) {
    used[id] = true;
  }

  for (auto id: parseCards(dead)) {
    used[id] = true;
  }

  for (const auto& hero: heroRange) {
    for (auto id: parseCards(hero)) {
      used[id] = true;
    }

    for (const auto& villain: villainRange) {
      for (auto id: parseCards(villain)) {
        used[id] = true;
      }

      for (int id = 1; id < 53; ++id) {
        if (used[id]) {
          continue;
        }

        if (isHoldem) {
          result = HoldemEvaluator::evalHands({hero, villain},
            board + toString({id}), dead);
        } else {
          result = OmahaEvaluator::evalHands({hero, villain},
            board + toString({id}), dead);
        }

        equities[id] += std::move(result[0].second);
        ++counts[id];
      }

      for (auto id: parseCards(villain)) {
        used[id] = false;
      }
    }

    for (auto id: parseCards(hero)) {
      used[id] = false;
    }
  }

  result.clear();

  for (int id = 1; id < 53; ++id) {
    if (!counts[id]) {
      continue;
    }

    result.emplace_back(toString({id}), equities[id] / counts[id]);
  }

  sort(result.begin(), result.end(),
    [&](const pair<string, double>& cardA, const pair<string, double>& cardB) {
      if (cardA.second != cardB.second) {
        return cardA.second > cardB.second;
      }

      return parseCards(cardA.first)[0] > parseCards(cardB.first)[0];
    });

  return result;
}
