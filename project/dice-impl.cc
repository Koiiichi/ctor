module dice;

import <iostream>;
import <string>;

using namespace std;

Dice::Dice(): rng{static_cast<unsigned>(seed)} {}

int LoadedDice::roll() {
  while (true) {
    cout << "Input a roll between 2 and 12:" << endl;
    cout << "> ";
    string input;
    if (!(cin >> input)) {
      return 0;
    }
    try {
      size_t consumed = 0;
      int value = stoi(input, &consumed);
      if (consumed == input.size() && value >= 2 && value <= 12) {
        return value;
      }
    }
    catch (...) {
    }
    cout << "Invalid roll." << endl;
  }
}

int FairDice::roll() {
  return (rng() % 6) + (rng() % 6) + 2;
}
