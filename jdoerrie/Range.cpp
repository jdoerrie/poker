#include "Range.h"

#include "Card.h"
#include "Evaluator.h"
#include "GameType.h"
#include "Hand.h"
#include "Utils.h"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

Range::Range(const string& description) : description_(description) {};
Range::Range(const vector<Hand>& hands) : hands_(hands) {};

size_t Range::fromRegEx(const string& str, GameType gameType) {
  gameType_ = gameType;
  vector<string> tokens;
  vector<regex> regexps;
  for (const auto& token: boost::split(tokens, str, boost::is_any_of(","))) {
    regexps.emplace_back(replaceIntervals(replaceSuitKeywords(token)),
                         regex::perl);
  }

  for (const Hand& hand: Hand::enumerateAllHands(gameType)) {
    for (auto regex: regexps) {
      if (regex_search(hand.toString(/* allRanksFirst */ true), regex)) {
        hands_.push_back(hand);
        break;
      }
    }
  }

  return hands_.size();
}


const vector<Hand>& Range::getHands() const {
  return hands_;
}

Range Range::filter(const Hand& hand) const {
  vector<Hand> newHands;
  for (const auto& hand_: hands_) {
    if ((hand.getId() & hand_.getId()) == 0) {
      newHands.push_back(hand_);
    }
  }

  return newHands;
}


string Range::replaceSuitKeywords(string str) {
  size_t numCards = Utils::getNumCards(gameType_);
  string suitStr = string(numCards, 's') + "|" + string(numCards, 'h') + "|" +
                   string(numCards, 'c') + "|" + string(numCards, 'd');

  string suited = "(" + suitStr + ")";
  string offsuit = "(?!" + suitStr + ").{" + to_string(numCards) + "}";

  size_t idx;
  while ((idx = str.find("su")) != string::npos) {
    str = str.replace(idx, 2, suited);
  }

  while ((idx = str.find("of")) != string::npos) {
    str = str.replace(idx, 2, offsuit);
  }

  return str;
}


string Range::replaceIntervals(string str) {
  size_t idx = str.find('-');
  if (idx == string::npos) {
    return str;
  }

  size_t numCards = Utils::getNumCards(gameType_);
  string start = str.substr(0, idx);
  string end = str.substr(idx + 1);

  if (start.size() != end.size() || start.size() < numCards ||
      start.substr(numCards) != end.substr(numCards)) {
    return str;
  }

  string suits = start.substr(numCards);
  vector<size_t> startIds, endIds;
  size_t maxDiff = 0;
  for (size_t i = 0; i < numCards; ++i) {
    startIds.push_back(static_cast<size_t>(Utils::toRank(start[i])));
    endIds.push_back(static_cast<size_t>(Utils::toRank(end[i])));

    if (startIds[i] < endIds[i]) {
      return str;
    }

    maxDiff = max(maxDiff, startIds[i] - endIds[i]);
  }

  for (size_t i = 0; i < numCards; ++i) {
    if ((startIds[i] - endIds[i]) % maxDiff != 0) {
      return str;
    }
  }


  string ranks = "(";
  for (size_t i = 0; i <= maxDiff; ++i) {
    for (size_t j = 0; j < numCards; ++j) {
      size_t currId = startIds[j] - i * (startIds[j] - endIds[j]) / maxDiff;
      ranks += Utils::toChar(static_cast<Rank>(currId));
    }

    ranks += (i != maxDiff) ? '|' : ')';
  }

  return ranks + suits;
}


void Range::normalize() {
  sort(hands_.rbegin(), hands_.rend());
  hands_.erase(unique(hands_.begin(), hands_.end()), hands_.end());
}
