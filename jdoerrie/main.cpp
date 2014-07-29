#include "Evaluator.h"
#include "OmahaEvaluator.h"
#include "HoldemEvaluator.h"
#include "Utils.h"

#include <cstdio>
#include <getopt.h>
#include <iostream>
#include <set>

using namespace std;

int main(int argc, char *argv[]) {

  bool success = Evaluator::initialize("../linux/HandRanks.dat");
  if (!success) {
    cerr << "Failed to initialize Evaluator, exiting." << endl;
    return 1;
  }

  string mode, game, hands, board, dead;

  static struct option long_options[] = {
    { "mode",  required_argument, NULL,  'm' },
    { "game",  required_argument, NULL,  'g' },
    { "hands", required_argument, NULL,  'h' },
    { "board", optional_argument, NULL,  'b' },
    { "dead",  optional_argument, NULL,  'd' },
    { NULL,                    0, NULL,   0  }
  };

  int ch = 0;
  while ((ch = getopt_long(argc, argv, "m:g:h:bd", long_options, NULL)) != -1) {
    switch (ch) {
      case 'm':
        mode = optarg;
        break;

      case 'g':
        game = optarg;
        break;

      case 'h':
        hands = optarg;
        break;

      case 'b':
        board = optarg;
        break;

      case 'd':
        dead = optarg;
        break;
    }
  }

  if (!mode.size() || !game.size() || !hands.size()) {
    cout << argv[0] << " --mode=(eval|card|hand) --game=(holdem|omaha) --hands"
      " [--board, --dead]\n";
    return 1;
  }

  vector<string> players = Utils::tokenize(hands, ',');
  vector<pair<string, double>> equities;

  if (mode == "eval") {
    if (game == "holdem") {
      equities = HoldemEvaluator::evalHands(players, board, dead);
    } else if (game == "omaha") {
      equities = OmahaEvaluator::evalHands(players, board, dead);
    }
  } else {
    equities = Evaluator::bestCardForHero({players[0]}, {players[1]},
      board, dead, game);
  }

  cout << "Board: " << board << endl;
  cout << "Dead: " << dead << endl;
  Evaluator::printEquities(equities);


  return 0;
}
