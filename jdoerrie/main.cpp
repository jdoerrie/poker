#include "PokerGame.h"
#include "Range.h"
#include "card.h"
#include "card_set.h"

#include <boost/algorithm/string.hpp>
#include <getopt.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
  ios_base::sync_with_stdio(false);

  string mode, game, handsStr;
  CardSet board, dead;

  static struct option long_options[] = {
      {"mode", required_argument, nullptr, 'm'},
      {"game", required_argument, nullptr, 'g'},
      {"hands", required_argument, nullptr, 'h'},
      {"board", optional_argument, nullptr, 'b'},
      {"dead", optional_argument, nullptr, 'd'},
      {nullptr, 0, nullptr, 0}};

  int ch = 0;
  while ((ch = getopt_long(argc, argv, "m:g:h:b:d:", long_options, nullptr)) !=
         -1) {
    switch (ch) {
      case 'm':
        mode = optarg;
        break;

      case 'g':
        game = optarg;
        break;

      case 'h':
        handsStr = optarg;
        break;

      case 'b':
        board = CardSet(optarg);
        break;

      case 'd':
        dead = CardSet(optarg);
        break;
    }
  }

  if (mode.empty() || game.empty() || handsStr.empty()) {
    cout << argv[0]
         << " --mode=[eval, card, hand, classes] "
            "--game=[holdem, omaha, omaha5] --hands [--board, --dead]\n";
    return 1;
  }

  vector<string> players;
  boost::split(players, handsStr, boost::is_any_of(":"));
  vector<CardSet> hands;
  for (const auto& player : players) {
    hands.emplace_back(player);
  }

  GameType gameType = getGameType(game);
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

  cout << "Board: " << board << endl;
  cout << "Dead: " << dead << endl;
  cout << endl;

  PokerGame pokerGame(gameType, ranges, board, dead);
  if (mode == "eval") {
    pokerGame.printEquities();
  } else if (mode == "card") {
    pokerGame.printNextCards();
  } else if (mode == "hand") {
    pokerGame.printRangeBreakdown();
  } else if (mode == "classes") {
    cout << "Value Hands:\n";
    pokerGame.printCategories();
    cout << "\nDraws:\n";
    pokerGame.printDraws(0.05);
  }

  return 0;
}
