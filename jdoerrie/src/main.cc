#include <iostream>

#include <boost/algorithm/string.hpp>
#include <gflags/gflags.h>

#include "card.h"
#include "card_set.h"
#include "poker_game.h"
#include "range.h"

using namespace std;

DEFINE_string(
    mode,
    "eval",
    "Mode of the evaluator. Should be one of {eval, card, hand, classes}");
DEFINE_string(
    game,
    "holdem",
    "Type of the poker game. Should be one of {holdem, omaha, omaha5}");
DEFINE_string(hands,
              "AAsh:KKsh",
              "Colon separated string of starting hands. Understands ranges.");
DEFINE_string(board, "", "Community cards.");
DEFINE_string(dead, "", "Dead cards.");

int main(int argc, char* argv[]) {
  ios_base::sync_with_stdio(false);
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  string mode, game, handsStr;
  CardSet board(FLAGS_board), dead(FLAGS_dead);

  vector<string> players;
  boost::split(players, FLAGS_hands, boost::is_any_of(":"));

  GameType gameType = getGameType(FLAGS_game);
  if (gameType == GameType::NONE) {
    cerr << "ERROR: " << game << " is not a valid game" << endl;
    return 1;
  }

  vector<Range> ranges(players.size());
  for (size_t i = 0; i < players.size(); ++i) {
    cout << "Player " << i + 1 << ": " << players[i];
    int combs = ranges[i].fromRegEx(players[i], gameType);
    ranges[i] = ranges[i].filter(board).filter(dead);
    cout << "\t(" << ranges[i].getHands().size() << "/" << combs
         << " combos)\n";
  }

  cout << "Board: " << FLAGS_board << endl;
  cout << "Dead: " << FLAGS_dead << endl;
  cout << endl;

  PokerGame pokerGame(gameType, ranges, board, dead);
  if (FLAGS_mode == "eval") {
    pokerGame.printEquities();
  } else if (FLAGS_mode == "card") {
    pokerGame.printNextCards();
  } else if (FLAGS_mode == "hand") {
    pokerGame.printRangeBreakdown();
  } else if (FLAGS_mode == "classes") {
    cout << "Value Hands:\n";
    pokerGame.printCategories();
    cout << "\nDraws:\n";
    pokerGame.printDraws(0.05);
  }

  return 0;
}
