#include "Range.h"

#include "Card.h"
#include "Evaluator.h"
#include "Hand.h"
#include "Utils.h"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

Range::Range(const string& description) : description_(description) {};
Range::Range(const vector<Hand>& hands) : hands_(hands) {};

size_t Range::fromRegEx(const string& str, GameType gameType) {
  vector<string> tokens;
  vector<regex> regexps;
  for (const auto& token: boost::split(tokens, str, boost::is_any_of(","))) {
    regexps.emplace_back(token, regex::perl);
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
